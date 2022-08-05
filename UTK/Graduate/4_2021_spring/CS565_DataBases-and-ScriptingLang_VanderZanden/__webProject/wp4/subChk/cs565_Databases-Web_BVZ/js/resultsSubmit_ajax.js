$("#resultsRow6-1").click(
    function()
    {

        // loginData is a string, not an array b/c I used JSON.stringify in login.js
        // must convert this to array again.
        let sections = window.sessionStorage.getItem("login");
        let selectedSection = $("#nav_DropDown").find(":selected").text();
        let optionStr = "";
        let resultsData = {};
        let selectedOutcome = $("#nav_Outcomes > a.outcome.selectedOutcome").text();
        let notMeetsExpectations = $("#resultsRow5-1");
        let meetsExpectations = $("#resultsRow5-2");
        let exceedsExpectations = $("#resultsRow5-3");
        let readyToSubmit = true;
        let originalBorder = "1px solid rgb(192, 192, 192)";
        let orginalShadow = "none"; // box-shadow
        let originalStyles = {"border": originalBorder, "box-shadow": orginalShadow};
        let errorBorder = "1px solid red";
        let errorShadow = "0 0 3px 2px red" // box-shadow
        let errorStyles = {"border": errorBorder, "box-shadow": errorShadow};
        let $resultErr = $("#resultErr");
        let maxHeight = "10px";
        let animationSpeed = 500;
        let minHeight = "0px";

        // create array of JSON objects
        // reassign sections to it
        sections = JSON.parse(sections);

        // extract AJAX data from the DOM
        //  outcome number: split by any whitespace into an array and then pop last element b/c
        // ...the last word in string has the needed number.
        let outcomeId = parseInt( selectedOutcome.split(/\s+/).pop() );
        let performanceLevel = 0; // will be incremented in loop from 1 - 3
        let numberOfStudents = [$(notMeetsExpectations).val(), 
                                $(meetsExpectations).val(), 
                                $(exceedsExpectations).val()];

        let numberOfStudentsObj = [$(notMeetsExpectations), 
            $(meetsExpectations), 
            $(exceedsExpectations)];
    
        let resetWtsDescrsStyles = function()
        {
            $("#resultsRow5-1").removeAttr( "style" );
            $("#resultsRow5-2").removeAttr( "style" );
            $("#resultsRow5-3").removeAttr( "style" );
        }

        let removeModifiedFlags = function()
        {
            // get assessment inputs/textareas
            $(notMeetsExpectations).removeClass("modified");
            $(meetsExpectations).removeClass("modified");
            $(exceedsExpectations).removeClass("modified");
        }

        // do error checking
        // negative numbers
        numberOfStudentsObj.forEach(
            function(obj, index)
            {
                if ($(obj).val() < 0) 
                {
                    $(obj).css( errorStyles );
                    readyToSubmit = false;

                }
            }
        );

        if (readyToSubmit)
        {
            //ANIMATIONS------------------------------------------------------------
            $resultErr.css("color", "rgb(87, 186, 88)");
            $resultErr.text("Results successfully saved!");
            
            $resultErr.animate( {height: maxHeight}, animationSpeed,
                function()
                {
                    $resultErr.delay(4*animationSpeed).animate( {height: minHeight}, animationSpeed,
                        function()
                        {
                            $resultErr.text("");
                        }    
                    );
                    }
            );

            sections.forEach(
                function(obj, index)
                {
                    optionStr = obj["courseId"] +" "+ obj["semester"] +" "+ obj["year"] +" "+ obj["major"];
                    
                    // once we find a match, extract needed data and send AJAX request. Then return.
                    if (optionStr === selectedSection) 
                    {
                        // remove modified flags from results inputs
                        removeModifiedFlags();
                        resetWtsDescrsStyles();

                        // extract remaining AJAX data
                        let sectionId = obj["sectionId"];
                        let major = obj["major"];

                        // make ajax calls for each performanceLevel (1, 2, 3)
                        for (let i = 0; i < 3; i++)
                        {
                            performanceLevel++;

                            resultsData = {"sectionId": sectionId, "outcomeId": outcomeId, "major": major,  "performanceLevel": performanceLevel, "numberOfStudents": numberOfStudents[i]};

                            $.ajax(
                                {
                                    url: "php_modules/updateResults.php",
                                    method: "POST",
                                    data: resultsData,
                                    dataType: "json"
                                }
                            ).done(
                                function()
                                {
                                    //TEST PRINT
                                    console.log("insertion of ", resultsData, " succeeded!");
                                }
                            ).fail(
                                function(jqXHR)
                                {
                                    $("#loginMsg").html("There was a problem contacting the server: <br>" +
                                    jqXHR.status + " " + jqXHR.responseText);
                                }
                            );

                        }
                    }

                }
            );

        }

    }
);