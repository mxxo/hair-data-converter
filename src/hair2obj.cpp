// convert hair data to obj
//
// vertex entries become
// v 0.0 0.1 0.2
// ...
// and strands become line elements built of vertex numbers
// l 1 2 3 4 5 6 7 ...
//
// -- Max Orok November 2019

#include "Hair.h"

#include <sstream>
using std::string;
using std::size_t;

void print_strand(std::ostream &oss, size_t start, size_t end_num) {
	oss << "l";
	for (auto i = start; i < end_num; ++i) {
		oss << " " << i + 1;
	}
	oss << "\n";
}

int main(int argc, char **argv) {

	if (argc != 3) {
		puts("usage: ./hair2obj <filename.data> <output>");
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

	auto strands = h.strands;

	// make a line element buffer
	std::ostringstream oss;

	int nverts = 0;
	for (auto &strand: strands) {
		auto strand_verts = strand.size();
		// print vertex information
		for (size_t i = 0; i < strand_verts; ++i) {
			out_file << "v "
				 << strand[i][0] << " "
				 << strand[i][1] << " "
				 << strand[i][2] << "\n";
		}

		// print strand vertex numbers to strand buffer
		auto total_verts = nverts + strand_verts;
		print_strand(oss, nverts, total_verts);
		nverts = total_verts;
	}

	// toss strand buffer into out file
	out_file << oss.str();
	out_file.close();

	return 0;
}
