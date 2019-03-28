function msg_int(a) {
	var writedictdata = ["set", "post_data", a.toString()];
	//var outputFile = ["set", "filename_out", "~/Desktop/data/" + a.toString() + ".json"];
	outlet (0, writedictdata);
	//outlet (0, outputFile);
	outlet(0, "bang");
}