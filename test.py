import os
import sys

# Get the absolute path to the directory containing module.so
module_dir = os.path.abspath('/Users/vapostolop/Desktop/pyRoPE/build')

# Add the module directory to sys.path
sys.path.append(module_dir)

# Now you can import functions from module.so
import moduleAtom as ma
import moduleEnv as menv

def main():

	# open_json(name_of_file);
	# mm = model_manager();

	kk = ma.greet()
	print(kk)
	# Create an AtomObject
	atom = ma.create_atom()
	position = ma.get_one_atom_position(atom)
	print(position)
	connected_atom = ma.create_atom()
	# connected_atom = module.get_connected_atoms(atom, 0)
	# print(connected_atom)
	# isatom = module.is_connected_atom(atom, connected_atom)
	ma.set_derived_position(atom, position)
	print(type(atom))
	ma.get_atom_name(atom)
	ma.get_atom_num(atom)
	ma.get_desc(atom)
	ma.get_bond_length(atom)
	menv.load_env("rope_demo.json")
	model_manager = menv.get_model_manager()
	print(type(model_manager))



if __name__ == "__main__":
    main()