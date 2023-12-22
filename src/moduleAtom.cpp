#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <string>
#include <vagabond/core/Atom.h>
#include "moduleAtom.h"


PyMethodDef methods[] = 
{
    {"greet", greet, METH_VARARGS, "terrible documentation"},
    {"create_atom", createAtomObject, METH_NOARGS, "Create an atom object"},
    {"get_one_atom_position", getOneAtomPosition, METH_O, "Get one atom position"},
    {"get_connected_atoms", getConnectedAtom, METH_VARARGS, "Get connectedAtom"},
    {"set_derived_position", setDerivedPosition,  METH_VARARGS, "Set derived position"},
    {"is_connected_atom", getIsConnectedAtom, METH_VARARGS, "Check if there is a connected atom"},
    {"get_atom_name", getAtomName, METH_VARARGS, "Get the name of atom position"},
    {"get_atom_num", getAtomNum, METH_VARARGS, "Get the number of atom position"},
    {"get_desc", getDesc, METH_VARARGS, "Get the desc"},
    {"get_bond_length", getBondLengthCount, METH_VARARGS, "Get bond length"},
    {NULL, NULL, 0, NULL}
};

// Initiallisations and required methods
struct PyModuleDef module_def = 
{
    PyModuleDef_HEAD_INIT,
    .m_name = "moduleAtom",
    .m_doc = "Helen Module",
    .m_size = -1,
    .m_methods = methods,
};

PyMODINIT_FUNC PyInit_moduleAtom(void)
{
	// return PyModule_Create(&module_def);

	PyObject *m = PyModule_Create(&module_def);

	if (m)
	{
		m = helen_module_py_init(m);
	}

	return m;
}

PyMODINIT_FUNC helen_module_py_init(PyObject *module)
{
    return module;
}

// Wrapping Atom methods
PyObject* greet(PyObject* self, PyObject * args)
{
    return Py_BuildValue("s", "Hello world");
}

PyObject* createAtomObject(PyObject* self, PyObject* args)
{
 
    Atom* atom = new Atom(); // not sure about this line
    std::cout << "Before packing: " << atom << std::endl;
    //PyObject* atomPointer = Py_BuildValue("O&", atom);
    PyObject* atomPointer = PyCapsule_New(atom, nullptr, nullptr);


    return atomPointer;

}

PyObject* getOneAtomPosition(PyObject* self, PyObject* args)
{
    void* val = PyCapsule_GetPointer(args, NULL);
    Atom *atom = static_cast<Atom *>(val);
    std::cout << "After packing: " << atom << std::endl;
    glm::vec3 position = atom->derivedPosition();
    PyObject* positionTuple = Py_BuildValue("(fff)", position.x, position.y, position.z);

    return positionTuple;
}

PyObject* setDerivedPosition(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    PyObject* positionTuple;
    // passing by as argument a positionTuple from python and need to convert the type to glm::vec3 &pos 

    // parsing the two arguments: atom and position tuple:
    std::cout << "Parse arguments" << std::endl;
    if (!PyArg_ParseTuple(args, "OO", &atomCapsule, &positionTuple)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* val = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom *atom = static_cast<Atom *>(val);

    // Extract elements from the position tuple
    float x, y, z;
    if (!PyArg_ParseTuple(positionTuple, "fff", &x, &y, &z)) {
        PyErr_SetString(PyExc_TypeError, "Invalid position tuple");
        return NULL;
    }
    glm::vec3 position(x, y, z);
    atom->setDerivedPosition(position);
     // Print the values for verification
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

    Py_RETURN_NONE;
}



PyObject* getConnectedAtom(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    int i;

    std::cout << "Parse arguments" << std::endl;
    if (!PyArg_ParseTuple(args, "Oi", &atomCapsule, &i)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    std::cout<< "Connetion atom number: " <<std::endl;
    std::cout<< i <<std::endl;
    std::cout << "Retrieve atom object from the capsule" << std::endl;
    void* val = PyCapsule_GetPointer(atomCapsule, NULL);
    std::cout<< "Flag 1" <<std::endl;
    if (!val) {
        PyErr_SetString(PyExc_TypeError, "Invalid capsule");
        return NULL;
    }
    Atom *atom = static_cast<Atom *>(val);
    std::cout<< "Flag 2" << atom << std::endl;
    size_t count = atom->bondLengthCount(); // this function is found in HasBondstraints.h
    std::cout<< "Flag 3 " << count <<std::endl;
    if (i >= count) {
        PyErr_SetString(PyExc_RuntimeError, "Count small than requested number");
        return NULL;
    }
    Atom* connectedAtom = atom->connectedAtom(i);
    PyObject* result = PyCapsule_New(connectedAtom, NULL, NULL);
    if (!result) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create capsule");
        return NULL;
    }

    return result;

}


PyObject* getIsConnectedAtom(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    PyObject* connectedAtomCapsule;

    if (!PyArg_ParseTuple(args, "OO", &atomCapsule, &connectedAtomCapsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* atomVal = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom* atom = static_cast<Atom*>(atomVal);

    void* connectedAtomVal = PyCapsule_GetPointer(connectedAtomCapsule, NULL);
    Atom* connectedAtom = static_cast<Atom*>(connectedAtomVal);

    bool result = atom->isConnectedToAtom(connectedAtom);
    return PyBool_FromLong(result);
}


PyObject* getAtomName(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    if (!PyArg_ParseTuple(args, "O", &atomCapsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* atomVal = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom* atom = static_cast<Atom*>(atomVal);

    const std::string& atomName = atom->atomName();
    std::cout << "Getting atom name: " << atomName << std::endl;

    // You can return the atom name as a Python string if needed
    return Py_BuildValue("s", atomName.c_str());
}

PyObject* getAtomNum(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    if (!PyArg_ParseTuple(args, "O", &atomCapsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* atomVal = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom* atom = static_cast<Atom*>(atomVal);

    const int& atomNum = atom->atomNum();
    std::cout << "Getting atom num: " << atomNum << std::endl;

    // You can return the atom name as a Python string if needed
    return Py_BuildValue("i", atomNum);
}

PyObject* getDesc(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    if (!PyArg_ParseTuple(args, "O", &atomCapsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* atomVal = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom* atom = static_cast<Atom*>(atomVal);

    const std::string& descChain= atom->desc();
    std::cout << "Getting desc: " << descChain << std::endl;

    // You can return the atom name as a Python string if needed
    return Py_BuildValue("s", descChain.c_str());
}

PyObject* getBondLengthCount(PyObject* self, PyObject* args)
{
    PyObject* atomCapsule;
    if (!PyArg_ParseTuple(args, "O", &atomCapsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    void* atomVal = PyCapsule_GetPointer(atomCapsule, NULL);
    Atom* atom = static_cast<Atom*>(atomVal);

    const size_t bondLengths = atom->bondLengthCount();
    std::cout << "Getting bond length count: " << bondLengths << std::endl;

    return Py_BuildValue("n", bondLengths);
}









