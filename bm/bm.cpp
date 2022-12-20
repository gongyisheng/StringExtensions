#include <Python.h>

#define ALPHABET_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))
// #define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

// get good suffix table
static void getGoodSuffix(const char *pattern, int *goodsuffix, int plen){
    if(plen==0){
        return;
    }
    if(plen==1){
        goodsuffix[0] = 1;
    }

    for(int i=0; i<plen; i++){
        goodsuffix[i] = 0;
    }

    int i = plen-2;
    int j = plen-1;
    
    while(i >= 0){
        if(pattern[i]!=pattern[j]){
            j = plen-1;
        }
        else{
            if(goodsuffix[j]==0){
                goodsuffix[j] = j-i;
            }
            j--;
        }
        i--;
    }
    #ifdef DEBUG
    std::cout << "Good Suffix Table: ";
    for(int k=0; k<plen; k++){
        std::cout << goodsuffix[k] << ",";
    }
    std::cout << "\n";
    #endif
}

// get bad char table
static void getBadChar(const char *pattern, int *badmatch, int plen){
    for(int i=0; i<ALPHABET_SIZE; i++){
        badmatch[i] = -1;
    }

    for(int i=0; i<plen; i++){
        badmatch[(int)pattern[i]] = i;
    }

    #ifdef DEBUG
    std::cout << "Bad Char Table: ";
    for(int k=0; k<ALPHABET_SIZE; k++){
        if(badmatch[k]!=-1){
            std::cout << (char)k << "=" << badmatch[k] << ",";
        }
    }
    std::cout << "\n";
    #endif
}

// search text by pattern
static int search(const char *text, const char *pattern) {
    int plen = strlen(pattern);
    int tlen = strlen(text);
    if(plen==0 || tlen==0){
        return -1;
    }
    int badmatch[ALPHABET_SIZE];
    int goodsuffix[plen];

    getBadChar(pattern, badmatch, plen);
    getGoodSuffix(pattern, goodsuffix, plen);

    int i = 0, j = 0;
    while(i <= tlen-plen){
        #ifdef DEBUG
        std::cout << "Before: i=" << i << ", j=" << j << "\n";
        #endif
        j = plen-1;
        while(j>=0 && text[i+j]==pattern[j]){
            j--;
        }
        if(j<0){
            return i;
        }
        else{
            i += MAX(1,MAX(j-badmatch[(int)text[i+j]], goodsuffix[j]));
        }
        #ifdef DEBUG
        std::cout << "After: i=" << i << ", j=" << j << "\n";
        #endif
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
    int result = search(text, pattern);
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