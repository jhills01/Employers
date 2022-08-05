// #narraRow3 is my stupid name for this form. 
$("#narraRow6_btn").click(
    function()
    {
        // loginData is a string, not an array b/c I used JSON.stringify in login.js
        // must convert this to array again.
        let sections = window.sessionStorage.getItem("login");
        let selectedSection = $("#nav_DropDown").find(":selected").text();
        let optionStr = "";
        let narraData = {};

        // create array of JSON objects
        // reassign sections to it
        sections = JSON.parse(sections);

        // extract AJAX data from the DOM
        let selectedOutcome = $("#nav_Outcomes > a.outcome.selectedOutcome").text();
        // to get outcome number, split by any whitespace into an array and then pop last element b/c
        // ...the last word in string has the needed number
        let outcomeId = parseInt( selectedOutcome.split(/\s+/).pop() );
        let strengths = $("#narraRow3_textArea").val();
        let weaknesses = $("#narraRow4_textArea").val();
        let actions = $("#narraRow5_textArea").val();

        let readyToSubmit = false;
        let originalBorder = "1px solid rgb(192, 192, 192)";
        let orginalShadow = "none"; // box-shadow
        let originalStyles = {"border": originalBorder, "box-shadow": orginalShadow};
        let errorBorder = "1px solid red";
        let errorShadow = "0 0 3px 2px red" // box-shadow
        let errorStyles = {"border": errorBorder, "box-shadow": errorShadow};
        let $resultErr = $("#resultErr");
        let maxHeight = "10px";
        let minHeight = "0px";
        let maxHeightInt = 10;
        let minHeightInt = 0;
        let animationSpeed = 500;
        let $narraErr =  $("#narraErr");
        let currHeight = $narraErr.height();


        // removes all input modification flags
        let removeModifiedFlags = function()
        {
            // get assessment inputs/textareas
            $("#narraRow3_textArea").removeClass("modified");
            $("#narraRow4_textArea").removeClass("modified");
            $("#narraRow5_textArea").removeClass("modified");
        }

        // empty strings should evaluate to false
        if ( !strengths || !weaknesses)
        {
            $narraErr.css("color", "red");
            $narraErr.text("Strenghts and weaknesses may not be blank");
        }
        else
        {
            $narraErr.css("color", "rgb(87, 186, 88)");
            $narraErr.text("Narrative saved successfully!");

            readyToSubmit = true;
        }

        if (currHeight === minHeightInt)
        {
            $narraErr.animate( {height: maxHeight}, animationSpeed);
        }

        // call ajax
        if (readyToSubmit)
        {
            // close error msg
            $narraErr.delay(4*animationSpeed).animate( {height: minHeight}, animationSpeed,
                function()
                {
                    $narraErr.text("");
                }    
            );

            sections.forEach(
                function(obj, index)
                {
                    optionStr = obj["courseId"] +" "+ obj["semester"] +" "+ obj["year"] +" "+ obj["major"];

                    // once we find a match, extract needed data and send AJAX request. Then return.
                    if (optionStr === selectedSection) 
                    {
                        // before saving, remove modified flags
                        removeModifiedFlags();

                        // extract remaining AJAX data
                        let sectionId = obj["sectionId"];
                        let major = obj["major"];

                        narraData = {"sectionId": sectionId, "major": major, "outcomeId": outcomeId, "strengths": strengths, "weaknesses": weaknesses, "actions": actions};


                        // send request to server to inserting/updating assessments
                        $.ajax(
                            {
                                url: "php_modules/updateNarrative.php",
                                method: "POST",
                                data: narraData,
                                dataType: "json"
                            }
                        ).done(
                            function()
                            {
                                //TEST PRINT
                                console.log("insertion of ", narraData, " succeeded!");
                            }
                        ).fail(
                            function(jqXHR)
                            {
                                $("#loginMsg").html("There was a problem contacting the server: <br>" +
                                jqXHR.status + " " + jqXHR.responseText);
                            }
                        );


                        //TEST PRINT
                        {


                            // console.log("sectionId: ", sectionId);
                            // console.log("    type: ", typeof(sectionId));
                            
                            // console.log("major: ", major);
                            // console.log("    type: ", typeof(major));

                            // console.log("outcomeId: ", outcomeId);
                            // console.log("    type: ", typeof(outcomeId));

                            // console.log("strengths: ", strengths);
                            // console.log("    type: ", typeof(strengths));

                            // console.log("weaknesses: ", weaknesses);
                            // console.log("    type: ", typeof(weaknesses));

                            // console.log("actions: ", actions);
                            // console.log("    type: ", typeof(actions));
                        }

                        // Correct section found. no need to continue. Return.
                        return;
                        
                    } 
                }
            );
        }
    }
);
