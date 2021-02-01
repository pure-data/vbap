#pragma once

#include <math.h>
#include "m_pd.h"


// umlaeute: helper macros to ease compat between Pd and Max-versions
#ifdef PD

/* Pd doesn't have longs */
# define SETLONG SETFLOAT

# define atom_getlong(atom)          atom_getfloatarg(0, 1, atom)
# define atom_getsym(atom)           atom_getsymbolarg(0, 1, atom)
# define object_alloc(obj_class)     pd_new(obj_class)
# define object_free(obj)            pd_free((t_pd*)obj)
# define newobject(class)            pd_new(class)
# define outlet_int(outlet, number)  outlet_float(outlet, number)

# else // MAX

/* name changes */
typedef Atom t_atom;
typedef Symbol t_symbol;
# define SETSYMBOL SETSYM

/// enable/disable traces
static bool _enable_trace = false;
void traces(t_def_ls *x, long n) { _enable_trace = n ? true : false;}

/// pd_error -> post
# define pd_error(x, ...) post(__VA_ARGS__)

#endif // PD vs MAX



#ifndef M_PI
// don't know where it is in win32, so add it here
# define M_PI 3.14159265358979323846264338327950288
#endif


// maximum number of loudspeaker sets (triplets or pairs) allowed,
// revised by Z. Settel to dynamically allocate memory
#ifndef MAX_LS_SETS
# ifdef PD
// allows for up to 44 speakers in 3D config
#  define MAX_LS_SETS 745
# else // MAX
// this can crash when too many speakers are defined
#  define MAX_LS_SETS 100
# endif // PD vs MAX
#endif // MAX_LS_SETS

// maximum amount of loudspeakers, can be increased,
// but see MAX_LS_SETS comments above
#ifndef MAX_LS_AMOUNT
# define MAX_LS_AMOUNT 55
#endif

#define MATRIX_DIM          9 //< hard-coded matrix dimension for the algorithm
#define SPEAKER_SET_DIM     3 //< hard-coded speaker set dimension for the algorithm
#define MIN_VOL_P_SIDE_LGTH 0.01

#define VBAP_VERSION \
  "vbap v1.2.0 - 17 Jul 2018 - (c) Ville Pulkki 1999-2006 (Pd port by HCS)"
#define DFLS_VERSION \
  "define_loudspeakers v1.2.0 - 17 July 2018 - (c) Ville Pulkki 1999-2006"

static t_float rad2ang = 360.0 / (2.0f * M_PI);
static t_float atorad = (2.0f * M_PI) / 360.0f;

#ifdef VBAP_OBJECT
// inside vbap object, so sending matrix from define_loudspeakers
// is a simple call to the vbap receiver...
# define sendLoudspeakerMatrices(x, list_length, at) \
      vbap_matrix(x, gensym("loudspeaker-matrices"), list_length, at); \
      vbap_bang(x)
#else
// inside define_loudspeaker object, send matrix to outlet
# define sendLoudspeakerMatrices(x, list_length, at) \
    outlet_anything(x->x_outlet0, gensym("loudspeaker-matrices"), \
      list_length, at)
#endif

/// loudspeaker instance, distance value is 1.0 == unit vectors
typedef struct
{
  t_float x;       //< cartesian x coordinate
  t_float y;       //< cartesian y coordinate
  t_float z;       //< cartesian z coordinate
  t_float azi;     //< polar azimuth coordinate
  t_float ele;     //< polar elevation coordinate
  int channel_nbr; //< speaker channel number
} t_ls;

/// all loudspeaker sets
typedef struct t_ls_set
{
  int ls_nos[3];         //< channel numbers
  t_float inv_mx[9];     //< inverse 3x3 or 2x2 matrix
  struct t_ls_set *next; //< next set (triplet or pair)
} t_ls_set;

#ifdef VBAP_OBJECT
  typedef struct vbap
  {
    t_object x_obj;  //< object reference
    t_float x_azi;   //< panning direction azimuth
    t_float x_ele;   //< panning direction elevation
    void *x_outlet0;
    void *x_outlet1;
    void *x_outlet2;
    void *x_outlet3;
    void *x_outlet4;

    long x_lsset_available; //< were ls sets defined with define_loudspeakers?
    long x_lsset_amount;    //< amount of loudspeaker sets
    long x_ls_amount;       //< amount of loudspeakers
    long x_dimension;       //< 2 or 3

#ifdef PD
    // memory for data sets is now allocated dynamically in each instance

    t_float **x_set_inv_matx; //< inverse matrice for each loudspeaker set
    t_float **x_set_matx;     //< matrice for each loudspeaker set
    long **x_lsset;           //< channel numbers of loudspeakers in each ls set

    // original static sizing WAS:
    //t_float x_set_inv_matx[MAX_LS_SETS][9];
    //t_float x_set_matx[MAX_LS_SETS][9];
    //long x_lsset[MAX_LS_SETS][3];

    t_float x_spread; //< speading amount of virtual source (0-100)
#else // MAX
    // dynamic memory allocation not tested for max, so it is allocated in the
    // struct, as it was before

    /// inverse matrice for each loudspeaker set
    t_float x_set_inv_matx[MAX_LS_SETS][9];

    /// matrice for each loudspeaker set
    t_float x_set_matx[MAX_LS_SETS][9];

    /// channel numbers of loudspeakers in each LS set
    long x_lsset[MAX_LS_SETS][3];

    long x_spread; //< speading amount of virtual source (0-100)
    double x_gain; //< general gain control (0-2)
#endif // PD

    t_float x_spread_base[3]; //< used to create uniform spreading

    // define_loudspeaker data
    long x_ls_read;            //< 1 if loudspeaker directions have been read
    long x_triplets_specified; //< 1 if loudspeaker triplets have been chosen
    t_ls x_ls[MAX_LS_AMOUNT];  //< loudspeakers
    t_ls_set *x_ls_set;        //< loudspeaker sets
    long x_def_ls_amount;      //< number of loudspeakers
    long x_def_ls_dimension;   //< 2 (horizontal arrays) or 3 (3d setups)
    long x_ls_setCount;        //< the number of loudspeaker sets used for an
                               //< instance's current loudspeaker configuration
    long x_ls_set_current;     //< current active loudspeaker set
  } t_vbap;

  // define loudspeaker data type...
  typedef t_vbap t_def_ls;

#else // !VBAP_OBJECT

  /// define_loudspeakers Max MSP object
  typedef struct
  {
    t_object x_obj;            //< object reference
    long x_ls_read;            //< 1 if loudspeaker directions have been read
    long x_triplets_specified; //< 1 if loudspeaker triplets have been chosen
    t_ls x_ls[MAX_LS_AMOUNT];  //< loudspeakers
    t_ls_set *x_ls_set;        //< loudspeaker sets
    void *x_outlet0;           //< outlet
    long x_def_ls_amount;      //< number of loudspeakers
    long x_def_ls_dimension;   //< 2 (horizontal arrays) or 3 (3d setups)
  } t_def_ls;

#endif // VBAP_OBJECT


void vbap_def_ls(t_def_ls *x, t_symbol *s, int ac, t_atom *av);
