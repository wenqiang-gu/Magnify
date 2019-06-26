void celltree_to_magnify(const char* filename="../tools/CellTree/celltree.root");
void celltree_to_magnify(const char* filename)
{
    // gROOT->Reset();
    // gROOT->ProcessLine(".x loadClasses.C" );

	MagnifyWriter w(filename, "icarus", 0);
	// MagnifyWriter w(filename, 0, "icarus");
	w.PrintInfo();
	w.Write(0); // write first event

}
