// todo: add to uppercase, to lowercase use bit operations
#include <Python.h>

static char *toLowerCase(char *text) {
    char *p = text;
    while (*p) {
        *p |= 32;
        p++;
    }
    return text;
}

static char *toUpperCase(char *text) {
    char *p = text;
    while (*p) {
        *p &= -33;
        p++;
    }
    return text;
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
    char *text;
    
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    char *result = toLowerCase(text);
    return Py_BuildValue("s", result);
}

/* Convert string to upper case */
static PyObject *py_toUpperCase(PyObject *self, PyObject *args) {
    char *text;
    
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    char *result = toUpperCase(text);
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