//=============================================================================
// Convert hair strand data to a Gmsh msh file (v2.2)
// -- Max Orok, November 2019
//=============================================================================
// Overview:
//
// vertex entries become mesh nodes
// $Nodes
// <vert_num> 0.0 0.1 0.2
// ...
// $EndNodes
//
// and strands are built of single gmsh line elements using nodes
// $Elements
// <num elts>
// <elt_num> 1 2 0 1 <start_node> <end_node>
// ...
// $EndElements
//
// where 1 2 0 1 are magic numbers for gmsh line elements
//=============================================================================

#include "Hair.h"
#include <sstream>
using std::string;
using std::size_t;

// print line elements and return the number of elements printed
int print_strand(std::ostream &oss, size_t start, size_t end_num) {
	static int counter = 1; // running total held during every call

	auto magic_numbers = " 1 2 0 1 ";
	for (auto i = start; i < end_num; ++i) {
		// make a line from this node to the next
		oss << counter << magic_numbers << i << " " << i + 1 << '\n';
		counter++;
	}

	return counter - 1; // go back to regular counting
}

int main(int argc, char **argv) {

	if (argc != 3) {
		puts("usage: hair2gmsh <filename.data> <output>");
		exit(1);
	}

	// read file names
	auto f_name = argv[1];
	auto o_file = argv[2];

	// write output to file
	std::ofstream out_file(o_file);

	// make a new hair object
	Hair h;
	if (!h.read(f_name)) {
		printf("couldn't read %s", f_name);
		exit(1);
	}

	std::ostringstream node_buffer;
	std::ostringstream line_buffer;

	int nverts = 0;
	int num_lines = 0;
	int total_verts = 0;

	for (auto &strand: h.strands) {
		auto strand_verts = strand.size();
		// print vertex information
		for (size_t i = 0; i < strand_verts; ++i) {
			node_buffer << total_verts + i + 1 << " "
				 << strand[i][0] << " "
				 << strand[i][1] << " "
				 << strand[i][2] << "\n";
		}

		// print strand vertex numbers to strand buffer
		total_verts = nverts + strand_verts;
		// the +1 is to avoid starting from the old strand's end
		num_lines = print_strand(line_buffer, nverts + 1, total_verts);
		nverts = total_verts;
	}

	// dump buffers into Gmsh-formatted file

	// write the mesh file header
	out_file << "$MeshFormat\n" << 2.2 << " 0 " << " 8\n" << "$EndMeshFormat\n";

	// nodes
	out_file << "$Nodes\n";
	out_file << nverts << '\n'; // total number of nodes on top
	out_file << node_buffer.str();
	out_file << "$EndNodes\n";

	// line elements
	out_file << "$Elements\n";
	out_file << num_lines << '\n';
	out_file << line_buffer.str();
	out_file << "$EndElements\n";

	out_file.close();
	return 0;
}