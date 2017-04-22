var https = require('https');
var fs = require('fs');
var fs1 = require('fs');


var origins = [];
var units = 'metric';
var i;
var destinationUrl = "";
var start = 0;
var destinations = [];
var originUrl = [];
//reading origin from file where exactly the user lies.
fs1.readFile('userPoint.txt', 'utf8', function(err, data) {

    if (err) {
        return console.log(err);
    } else {
        tokenDelim = data.indexOf(',', start);

        newLineDelim = data.indexOf('\n', start);

        origins.push({
            lat: data.substring(start, tokenDelim),
            long: data.substring(tokenDelim + 1, newLineDelim)
        });


        fs.readFile('coordinates.txt', 'utf8', function(err, data) {
            if (err) {
                return console.log(err);
            }
            // console.log(data);

            while (data.indexOf(',', start) != -1) {
                // console.log("start " + start);
                tokenDelim = data.indexOf(',', start);
                // console.log("td" + tokenDelim);
                newLineDelim = data.indexOf('\n', start);
                // console.log(newLineDelim);
                destinations.push({
                    lat: data.substring(start, tokenDelim),
                    long: data.substring(tokenDelim + 1, newLineDelim)
                });
                start = newLineDelim + 1;
            }

            for (i = 0; i < destinations.length; i++) {
                destinationUrl += destinations[i].lat + "%2C" + destinations[i].long;
                if (i != destinations.length - 1) {
                    destinationUrl += "%7C";
                }
            }
            for (i = 0; i < origins.length; i++) {
                originUrl += origins[i].lat + "," + origins[i].long;
            }

            // console.log(destinationUrl);
            // console.log(origins);

            https.get({
                    API_KEY: 'AIzaSyAaMeRFOJn341xS5r4jskYaWH-Yw9qtoiQ',
                    host: 'maps.googleapis.com',
                    path: '/maps/api/distancematrix/json?units=' + units + '&' + 'origins=' + originUrl + '&destinations=' + destinationUrl
                        // path: '/maps/api/distancematrix/json?origins=Vancouver+BC|Seattle&destinations=San+Francisco|Victoria+BC&mode=bicycling&language=fr-FR&key=AIzaSyAaMeRFOJn341xS5r4jskYaWH-Yw9qtoiQ'
                }, function(response) {
                    // Continuously update stream with data
                    console.log('STATUS: ' + response.statusCode);
                    // console.log(response);
                    var responseStr = "";
                    response.on("data", function(chunk) {
                        responseStr += chunk;
                    });
                    response.on("end", function() {
                        var distanceObj = JSON.parse(responseStr);
                        console.log(distanceObj.rows[0].elements);

                        distanceObj = distanceObj.rows[0].elements;
                        var resultStr = "";
                        for (i = 0; i < distanceObj.length; i++) {
                            if (distanceObj[i].status == 'OK')
                                resultStr += distanceObj[i].distance.text + '\n';
                            else
                                resultStr += "-1 km \n";
                        }

                        fs.writeFile("distances.txt", resultStr, function(err, data) {
                            if (err) {
                                console.log("Unable to write to file.\n")
                            }
                            console.log("Success");
                        });

                    })
                }

            );
        });
    }
});