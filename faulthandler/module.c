/*
 * Fault handler for SIGSEGV, SIGFPE, SIGBUS and SIGILL signals: display the
 * Python backtrace and restore the previous handler. Allocate an alternate
 * stack for this handler, if sigaltstack() is available, to be able to
 * allocate memory on the stack, even on stack overflow.
 */

#include "faulthandler.h"

#define VERSION 1

PyDoc_STRVAR(module_doc,
"faulthandler module.");

static PyMethodDef module_methods[] = {
    {"enable", (PyCFunction)faulthandler_enable_method, METH_NOARGS,
     PyDoc_STR("enable(): enable the fault handler")},
    {"disable", (PyCFunction)faulthandler_disable_method, METH_NOARGS,
     PyDoc_STR("disable(): disable the fault handler")},
    {"sigsegv", faulthandler_sigsegv, METH_VARARGS,
     PyDoc_STR("sigsegv(release_gil=False): raise a SIGSEGV signal")},
    {"sigfpe", (PyCFunction)faulthandler_sigfpe, METH_NOARGS,
     PyDoc_STR("sigfpe(): raise a SIGFPE signal")},
#ifdef FAULTHANDLER_HAVE_SIGBUS
    {"sigbus", (PyCFunction)faulthandler_sigbus, METH_NOARGS,
     PyDoc_STR("sigbus(): raise a SIGBUS signal")},
#endif
#ifdef FAULTHANDLER_HAVE_SIGILL
    {"sigill", (PyCFunction)faulthandler_sigill, METH_NOARGS,
     PyDoc_STR("sigill(): raise a SIGILL signal")},
#endif
    {NULL, NULL} /* terminator */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "faulthandler",
    module_doc,
    -1,
    module_methods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif


PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit_faulthandler(void)
#else
initfaulthandler(void)
#endif
{
    PyObject *m, *version;

#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&module_def);
#else
    m = Py_InitModule3("faulthandler", module_methods, module_doc);
#endif
    if (m == NULL) {
#if PY_MAJOR_VERSION >= 3
        return NULL;
#else
        return;
#endif
    }

    faulthandler_enable();

#if PY_MAJOR_VERSION >= 3
    version = PyLong_FromLong(VERSION);
#else
    version = PyInt_FromLong(VERSION);
#endif
    PyModule_AddObject(m, "version", version);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}

