function nodecomputation(response, postedData) {
    //var parsedData = postedData.substr(6);
    var parsedData = postedData;
    console.log("Requested a node computation with " + parsedData);
    response.writeHead(200, {
        "Content-Type": "json"
    });
    response.write(buildJsonString(parsedData));
    response.end();
}

function aleaArray(n) {
    n = Math.floor(Math.random() * n);
    aleaArr = [];
    for (var a = 0; a < n; a++) {
        aleaArr.push(Math.floor(Math.random() * 1000));
    }
    return aleaArr;
}

function buildJsonString(value) {
    var testNumber = Math.floor(Math.random() * 100000000);
    var jsonstring = '{\n' + '"metadata": "some kind of needed data",\n' + '"number": [' + value + '],\n"divisors": [' + findDivisors(value) + "]\n}";
    return jsonstring;
}

// expensive function to see nodejs working in background via shell
function findDivisors(a) {
    var divisors = [];
    for (var n = 1; n < a / 2 + 1; n++) {
        if (a % n == 0) {
            divisors.push(n);
            console.log(n);
        }
    }
    return divisors;
}

exports.nodecomputation = nodecomputation;