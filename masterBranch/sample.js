node_xj = require('xls-to-json');
var fs = require('fs');

node_xj({
    input: "sample.xls", // input xls 
    output: "output.json", // output json 
    //sheet: "sheetname" // specific sheetname 
}, function(err, result) {
    if (err) {
        console.error(err);
    } else {

        //console.log(result);
        fs.writeFile("mohit", result, function(err) {
            fs.EOL;
            if (err) {
                return console.log(err);
            }
            console.log("The file was saved!");
        });
    }
});