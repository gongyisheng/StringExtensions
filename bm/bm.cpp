#include <Python.h>
#include <algorithm>

// #define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

// get badmatch array
// static int *getBadMatch(const char *pattern) {
//     int *badmatch = new int[128];
//     int plen = strlen(pattern);
//     for(int i=0; i<128; i++){
//         badmatch[i] = -1;
//     }

//     for(int i=0; pattern[i]!=NULL; i++){
//         badmatch[(int)pattern[i]] = std::max(1, plen-i-1);
//     }

//     #ifdef DEBUG
//     std::cout << "Pattern length=" << plen << "\n";
//     std::cout << "Badmatch array: ";
//     for(int k=0; k<128; k++){
//         if(badmatch[k]!=-1){
//             std::cout << (char)k << "=" << badmatch[k] << ",";
//         }
//     }
//     std::cout << "\n";
//     #endif
//     return badmatch;
// }

// match text by pattern
static int match(const char *text, const char *pattern) {
    int badmatch[128];
    int plen = strlen(pattern);
    int tlen = strlen(text);
    for(int i=0; i<128; i++){
        badmatch[i] = -1;
    }

    for(int i=0; pattern[i]!=NULL; i++){
        badmatch[(int)pattern[i]] = std::max(1, plen-i-1);
    }
    #ifdef DEBUG
    std::cout << "Pattern length=" << plen << "\n";
    std::cout << "Badmatch array: ";
    for(int k=0; k<128; k++){
        if(badmatch[k]!=-1){
            std::cout << (char)k << "=" << badmatch[k] << ",";
        }
    }
    std::cout << "\n";
    #endif

    int i = 0, j = 0;
    while(i<tlen){
        #ifdef DEBUG
        std::cout << "i=" << i << ", text[i]=" << text[i] << "\n";
        #endif
        if(badmatch[(int)text[i]]!=-1){
            for(j=0; j<plen; j++){
                if(i-j<0 || text[i-j] != pattern[plen-j-1]){
                    i += badmatch[(int)text[i]];
                    break;
                }
            }
            if(j == plen){
                break;
            }
        }
        else{
            i += plen;
        }
    }

    if(j == plen){
        return i-j+1;
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
static PyObject *py_match(PyObject *self, PyObject *args) {
    const char *text;
    const char *pattern;
    if (!PyArg_ParseTuple(args, "ss", &text, &pattern)) {
        return NULL;
    }
    int result = match(text, pattern);
    return Py_BuildValue("i", result);
}


/* Module method table */
static PyMethodDef bmMethods[] = {
    {"match", py_match, METH_VARARGS, "Match text with pattern"},
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