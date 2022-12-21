#include <Python.h>

#define ALPHABET_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))
// #define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

static void preBmBc(const char *x, int m, int bmBc[]) {
   int i;
 
   for (i = 0; i < ALPHABET_SIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}
 
 
static void suffixes(const char *x, int m, int *suff) {
   int f, g, i;
 
   suff[m - 1] = m;
   g = m - 1;
   for (i = m - 2; i >= 0; --i) {
      if (i > g && suff[i + m - 1 - f] < i - g)
         suff[i] = suff[i + m - 1 - f];
      else {
         if (i < g)
            g = i;
         f = i;
         while (g >= 0 && x[g] == x[g + m - 1 - f])
            --g;
         suff[i] = f - g;
      }
   }
}
 
static void preBmGs(const char *x, int m, int bmGs[]) {
   int i, j, suff[m];
 
   suffixes(x, m, suff);
 
   for (i = 0; i < m; ++i)
      bmGs[i] = m;
   j = 0;
   for (i = m - 1; i >= 0; --i)
      if (suff[i] == i + 1)
         for (; j < m - 1 - i; ++j)
            if (bmGs[j] == m)
               bmGs[j] = m - 1 - i;
   for (i = 0; i <= m - 2; ++i)
      bmGs[m - 1 - suff[i]] = m - 1 - i;
}
 
 
static int search(const char *x, int m, const char *y, int n) {
   int i, j, bmGs[m], bmBc[ALPHABET_SIZE];
 
   /* Preprocessing */
   preBmGs(x, m, bmGs);
   preBmBc(x, m, bmBc);
 
   /* Searching */
   j = 0;
   while (j <= n - m) {
      for (i = m - 1; i >= 0 && x[i] == y[i + j]; --i);
      if (i < 0) {
         return j;
         j += bmGs[0];
      }
      else
         j += MAX(bmGs[i], bmBc[y[i + j]] - m + 1 + i);
   }
   return -1;
}


/* Python Wrapper Functions*/
/* Destructor function for points */
static void del_bmBadMatch(PyObject *obj) {
    free(PyCapsule_GetPointer(obj, NULL));
}

/* Utility functions */
// static bm_badmatch *Pybm_AsbmBadMatch(PyObject *obj) {
//     return (bm_badmatch *) PyCapsule_GetPointer(obj, "bm_badmatch");
// }

// static PyObject *Pykmp_FrombmBadMatch(bm_badmatch *badmatch, int must_free) {
//     return PyCapsule_New(badmatch, "bm_badmatch", must_free ? del_bmBadMatch : NULL);
// }

// match
static PyObject *py_search(PyObject *self, PyObject *args) {
    const char *text;
    const char *pattern;
    if (!PyArg_ParseTuple(args, "ss", &text, &pattern)) {
        return NULL;
    }
    int result = search(pattern, strlen(pattern), text, strlen(text));
    return Py_BuildValue("i", result);
}


/* Module method table */
static PyMethodDef bmMethods[] = {
    {"search", py_search, METH_VARARGS, "Search text with pattern"},
    {NULL, NULL, 0, NULL}
};

/* Module definition structure */
static struct PyModuleDef bmmodule = {
    PyModuleDef_HEAD_INIT,  /* m_base */
    "bm",                   /* name of module */
    "bm",                   /* module documentation, may be NULL */
    -1,                     /* size of per-interpreter state or -1 */
    bmMethods               /* method table */
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_bm(void) {
    Py_Initialize();
    return PyModule_Create(&bmmodule);
};