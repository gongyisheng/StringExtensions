// cannot improve performance because create a new string
#define LOWER_MASK 0x20
#define UPPER_MASK ~0x20
#include <Python.h>

static char *toLowerCase(const char *text) {
    char *p = new char[strlen(text)];
    for(int i=0;text[i]!=NULL;i++){
        p[i] = text[i] | LOWER_MASK;
    }
    return p;
}

static char *toUpperCase(const char *text) {
    char *p = new char[strlen(text)];
    for(int i=0;text[i]!=NULL;i++){
        p[i] = text[i] & UPPER_MASK;
    }
    return p;
}

/* Destructor function for points */
static void del_Util(PyObject *obj) {
    free(PyCapsule_GetPointer(obj, NULL));
}

/* Utility functions */
// static RadixNode *PyRadix_AsRadixNode(PyObject *obj) {
//     return (RadixNode *) PyCapsule_GetPointer(obj, "RadixNode");
// }

// static PyObject *PyRadix_FromRadixNode(RadixNode *node, int must_free) {
//     return PyCapsule_New(node, "RadixNode", must_free ? del_RadixNode : NULL);
// }

/* Convert string to lower case */
static PyObject *py_toLowerCase(PyObject *self, PyObject *args) {
    const char *text;
    
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    const char *result = toLowerCase(text);
    return Py_BuildValue("s", result);
}

/* Convert string to upper case */
static PyObject *py_toUpperCase(PyObject *self, PyObject *args) {
    const char *text;
    
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    const char *result = toUpperCase(text);
    return Py_BuildValue("s", result);
}


/* Module method table */
static PyMethodDef StringUtilsMethods[] = {
    {"toLowerCase", py_toLowerCase, METH_VARARGS, "convert string to lowercase"},
    {"toUpperCase", py_toUpperCase, METH_VARARGS, "convert string to uppercase"},
    {NULL, NULL, 0, NULL}
};

/* Module definition structure */
static struct PyModuleDef stringUtilsModule = {
    PyModuleDef_HEAD_INIT,    /* m_base */
    "stringUtils",            /* name of module */
    "String Utils",           /* module documentation, may be NULL */
    -1,                       /* size of per-interpreter state or -1 */
    StringUtilsMethods        /* method table */
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_stringUtils(void) {
    Py_Initialize();
    return PyModule_Create(&stringUtilsModule);
}