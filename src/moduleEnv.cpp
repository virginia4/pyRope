#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <string>
#include <vagabond/core/Environment.h>
#include <vagabond/core/ModelManager.h>
#include "moduleEnv.h"


PyMethodDef methods[] = 
{
    // {"create_model_env", createModelEnv, METH_NOARGS, "Create Model Environment"},
    {"get_model_manager", getModelManager, METH_NOARGS, "Get Model Manager"},
    {"load_env", loadEnvironment, METH_VARARGS, "Load form json file"},
    {NULL, NULL, 0, NULL}
};

// Initiallisations and required methods
struct PyModuleDef module_def = 
{
    PyModuleDef_HEAD_INIT,
    .m_name = "moduleEnv",
    .m_doc = "Environment Module",
    .m_size = -1,
    .m_methods = methods,
};

PyMODINIT_FUNC PyInit_moduleEnv(void)
{
	// return PyModule_Create(&module_def);

	PyObject *m = PyModule_Create(&module_def);

	if (m)
	{
		m = env_module_py_init(m);
	}

	return m;
}

PyMODINIT_FUNC env_module_py_init(PyObject *module)
{
    return module;
}

static PyObject* getModelManager(PyObject* self, PyObject* args)
{
    ModelManager* manager = Environment::modelManager();
    PyObject* managerCapsule = PyCapsule_New(manager, nullptr, nullptr);

    std::cout << manager->objectCount() << std::endl;
    return managerCapsule;
}


static PyObject* loadEnvironment(PyObject* self, PyObject* args)
{
    const char* file;

    // Parse the arguments to get the file path
    if (!PyArg_ParseTuple(args, "s", &file)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return nullptr;
    }

    // Call the original Environment::load method
    Environment::env().load(file);

    // You can return a result if needed
    Py_RETURN_NONE;
}










