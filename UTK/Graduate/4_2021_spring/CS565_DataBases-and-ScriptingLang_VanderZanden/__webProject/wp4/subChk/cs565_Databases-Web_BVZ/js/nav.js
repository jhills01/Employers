// DROP DOWN---------------------------------------------------------------------------------------------------------------------------

// loginData is a string, not an array b/c I used JSON.stringify in login.js
// must convert this to array again.
let loginData = window.sessionStorage.getItem("login");

// console.log(loginData);
// console.log(typeof(loginData));

// create array of JSON objects
loginData = JSON.parse(loginData);

// traverse each object in order and create dropdown options
let ndd = $("#nav_DropDown"); //find element only once. No need to traverse DOM tree for each option
let optionStr = "";
let dataArr = [];

loginData.forEach(
    function(obj) 
    {
        optionStr = obj["courseId"] +" "+ obj["semester"] +" "+ obj["year"] +" "+ obj["major"];
        // optionStrArr.push(optionStr);

// console.log("OPTIONSTR_preAJAX: " + optionStr);

        ndd.append(
            $("<option></option>").val(optionStr).html(optionStr)
        );

        //OUTCOMES ------------------------------------------------------------------------------------------------------------------
        $.ajax(
            {
                url: "php_modules/outcomes.php",
                method: "POST",
                //adding requestId in request so that I can identify corresponding response in done()
                data: {sectionId: obj["sectionId"], major: obj["major"], requestId: optionStr}, 
                dataType: "json",
            }
        ).done(
            function(data)
            {
                //no data returned b/c email/password was invalid
                if (data.length == 0)
                {
                    console.log("AJAX returned no data in nav.js: \n");
                    console.log("optionStr: " + optionStr);
                }

                //store returned data in clent-side sessionStorage array and load abet.php 
                // IMPORTANT NOTE: it's more efficient to store data in _SESSION array in PHP on the server side!!!!!
                else
                {
                    // // IMPORTANT NOTE: Major bug here!!! including string "data(parsed): " in console.log() made data not print correctly
                    // //                 This lead me to believe that JSON.parse wasn't working. wow.
                    // console.log(data);
                    // console.log("data(parsed): " + data);
                    
                    // store session variables
                    data = JSON.stringify(data);
                    // data = JSON.parse(data);
                    dataArr.push(data);
                    window.sessionStorage.setItem("outcomes", JSON.stringify(dataArr));
                }
            }
        ).fail(
            function(jqXHR)
            {
                console.log("There was a problem contacting the server: \n");
                console.log("jqXHR.status ==> " + jqXHR.status); 
                console.log("jqXHR.responseText ==> " + jqXHR.responseText);
            }
        );
    } 
);

// // TEST PRINT
// console.log("I should NOT last!!");
