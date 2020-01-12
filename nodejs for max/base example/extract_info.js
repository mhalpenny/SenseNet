outlets = 2;

var genomusDict = new Dict("urldict");

function bang() {
	outlet(0, genomusDict.get("body::number"));
	outlet(1, genomusDict.get("body::divisors"));
}
