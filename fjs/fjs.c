/*
   fjs.c
   FJS is a very fast algorithm for finding every occurrence
   of a string p of length m in a string x of length n.
   For details see <https://cgjennings.ca/articles/fjs/>.
   Christopher G. Jennings.
   See LICENSE.md for license details (MIT license).
*/
#include <Python.h>

// For large alphabets, such as Unicode, see the Web page above
// for techniques to improve performance

#define ALPHA      (256)       // alphabet size
#define MAX_PATLEN (100)       // maximum pattern length

int betap[MAX_PATLEN+1];
int Delta[ALPHA];


void makebetap(const char* p, int m) {
    int i = 0, j = betap[0] = -1;

    while(i < m) {
        while((j > -1) && (p[i] != p[j])) {
            j = betap[j];
        }
        if(p[++i] == p[++j]) {
            betap[i] = betap[j];
        } else {
            betap[i] = j;
        }
    }
}

void makeDelta(const char* p, int m) {
    int i;

    for(i = 0; i < ALPHA; ++i) {
        Delta[i] = m + 1;
    }
    for(i = 0; i < m; ++i) {
        Delta[p[i]] = m - i;
    }
}

int FJS(const char* p, int m, const char* x, int n) {
    if(m < 1) {
        return -1;
    }
    makebetap(p, m);
    makeDelta(p, m);

    int i = 0, j = 0, mp = m-1, ip = mp;
    while(ip < n) {
        if(j <= 0) {
            while(p[mp] != x[ip]) {
                ip += Delta[x[ip+1]];
                if(ip >= n){ 
                    return -1;
                }
            }
            j = 0;
            i = ip - mp;
            while((j < mp) && (x[i] == p[j])) {
                ++i; ++j;
            }
            if(j == mp) {
                return i-mp;
                ++i; ++j;
            }
            if(j <= 0) {
                ++i;
            } else {
                j = betap[j];
            }
        } else {
            while((j < m) && (x[i] == p[j])) {
                ++i; ++j;
            }
            if(j == m) {
                return i-m;
            }
            j = betap[j];
        }
        ip = i + mp - j;
    }
    return -1;
}

/* Python Wrapper Functions*/
/* Destructor function for points */
static void del_(PyObject *obj) {
    free(PyCapsule_GetPointer(obj, NULL));
}

/* Utility functions */
// static kmp_next *Pykmp_Askmpnext(PyObject *obj) {
//     return (kmp_next *) PyCapsule_GetPointer(obj, "kmp_next");
// }

// static PyObject *Pykmp_Fromkmpnext(kmp_next *next, int must_free) {
//     return PyCapsule_New(next, "kmp_next", must_free ? del_kmpnext : NULL);
// }

// match
static PyObject *py_search(PyObject *self, PyObject *args) {
    const char *text;
    const char *pattern;
    if (!PyArg_ParseTuple(args, "ss", &text, &pattern)) {
        return NULL;
    }
    int result = FJS(pattern, strlen(pattern), text, strlen(text));
    return Py_BuildValue("i", result);
}

/* Module method table */
static PyMethodDef fjsMethods[] = {
    {"search", py_search, METH_VARARGS, "search"},
    {NULL, NULL, 0, NULL}
};

/* Module definition structure */
static struct PyModuleDef fjsmodule = {
    PyModuleDef_HEAD_INIT,  /* m_base */
    "fjs",                  /* name of module */
    "fjs",                  /* module documentation, may be NULL */
    -1,                     /* size of per-interpreter state or -1 */
    fjsMethods              /* method table */
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_fjs(void) {
    Py_Initialize();
    return PyModule_Create(&fjsmodule);
};