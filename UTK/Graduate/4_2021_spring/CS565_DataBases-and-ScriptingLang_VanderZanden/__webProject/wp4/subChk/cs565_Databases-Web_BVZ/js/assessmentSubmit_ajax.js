// NOTE: no need to worry about preventingDefault for assessment submission. 
//       this behavior is already turned off for all buttons.
//       After checks are done, and all is good, use AJAX

let saveAss = $("#assRow6_btn"); // get save button for assessments

function assChecksAndSubmission ()
{
    let assError = $("#assError"); // capture error element
    let currHeight = assError.height(); // height attribute of error element
    let weights = $(".assessmentRow-1"); // get all assessments WEIGHTS in array
    let descriptions = $(".assessmentRow-2"); // get all assessments DESCRIPTIONS in array
    let blankWeights = [];
    let blankDescriptions = [];
    let maxHeight = "10px";
    let minHeight = "0px";
    let animationSpeed = 300; // milliseconds
    let sum = 0;
    let maxHeightInt = 10;
    let minHeightInt = 0;
    let originalBorder = "1px solid rgb(192, 192, 192)";
    let orginalShadow = "none"; // box-shadow
    let originalStyles = {"border": originalBorder, "box-shadow": orginalShadow};
    let errorBorder = "1px solid red";
    let errorShadow = "0 0 3px 2px red" // box-shadow
    let errorStyles = {"border": errorBorder, "box-shadow": errorShadow};

    let resetWtsDescrsStyles = function()
    {
        weights.each(
            function(index, htmlObj)
            {
                $(htmlObj).removeAttr( "style" );
                // $(htmlObj).css( "styles", originalStyles );
            }
        );

        descriptions.each(
            function(index, htmlObj)
            {

                $(htmlObj).removeAttr( "style" );
            }
        );
    }

    let wtSumChk = function() // returns true if weight sum === 100. False otherwise. 
    {
        let temp = 0;

        weights.each(
            function(index, htmlObj)
            {
                temp =parseInt(htmlObj["value"]);
                if (temp) {sum += temp;} // blanks cause NaN. This checks for it.
            }
        );

        if (sum !== 100) { return false; } 
        else { return true; }
    }

    let wtBlankChk = function () // returns array of DESCRIPTION elements that are blank
    {
        weights.each(
            function (index, htmlObj)
            {
                // "" evaluates to false
                if ( !htmlObj["value"] ) 
                { 
                    blankWeights.push(htmlObj); 
                }
            }
        );
    }
    
    let descrBlankChk = function () // returns array of DESCRIPTION elements that are blank
    {
        descriptions.each(
            function (index, htmlObj)
            {
// console.log("entered descriptions within descrBlankChk")                

                // "" evaluates to false
                if ( !htmlObj["value"] ) 
                { 
// console.log("found blank description: ", htmlObj);                    

                    blankDescriptions.push(htmlObj); 
                }
            }
        );
    }

    let wtBoundsChk = function() // returns true of a weight value is less than 1 or greater than 100
    {
        let value = 0;
        let outOfBounds = false;

        weights.each(
            function(index, obj)
            {
                value = obj["value"];
                if (value < 1 || value > 100) {outOfBounds = true; return;} 
            }
        );

        return outOfBounds;
    }

    let highLightOutOfBounds = function()
    {
        let value = 0;

        weights.each(
            function(index, obj)
            {
                value = obj["value"];
                if (value < 1 || value > 100) { $(obj).css( errorStyles ); } 
            }
        );
    }

    let highLightBlanks = function()
    {
        // change each blank description/weight's border to show which need attention
        blankDescriptions.forEach(
            function(htmlObj) 
            { 
                // $(htmlObj).css( {"border": errorBorder, "box-shadow": errorShadow} );
                $(htmlObj).css( errorStyles );
            }
        );

        blankWeights.forEach(
            function(htmlObj) 
            {
                // $(htmlObj).css( {"border": errorBorder, "box-shadow": errorShadow} ); 
                $(htmlObj).css( errorStyles ); 
            }
        );     
    }

    let updateDeleteAssessments = function()
    {
        // removes all input modification flags
        let removeModifiedFlags = function()
        {
            // get assessment inputs/textareas
            let inputs = $(".assessmentRow-1");
            let textareas = $(".assessmentRow-2");

            inputs.each(
                function (index, obj) { $(obj).removeClass("modified");  }
            );
            
            textareas.each(
                function (index, obj) { $(obj).removeClass("modified");  }
            );
        }
        
        // deletes all assessments stored in "deletedAssessment"
        let updateAssessments = function()
        {
            let assData = {};

            let sections = window.sessionStorage.getItem("login"); // major, sectionId
            let selectedSection = $("#nav_DropDown").find(":selected").text();
            let optionStr = "";
            let selectedOutcome = $("#nav_Outcomes > a.outcome.selectedOutcome").text();
            let assessments = $(".assessmentRow"); // get assessment rows
            let assessmentClasses = "";
            
            let assessmentId = null;
            let sectionId = 0;
            let major = "";
            let outcomeId = parseInt( selectedOutcome.split(/\s+/).pop() );
            let assessmentDescription = "";
            let weight = 0;

            // check to see if there are any weights/descriptions to even insert. There could be none
            // if ( assessments.length )
            // WAIT! This doesn't need to be done! Error checking doesn't allow saving blank assessments!
            // In other words, you can't save until all assessment requirements are met. So check has already been done.

            // create array of JSON objects
            // reassign sections to it
            sections = JSON.parse(sections);

            // determine and store current sectionId and major
            sections.forEach(
                function(obj, index)
                {
                    optionStr = obj["courseId"] +" "+ obj["semester"] +" "+ obj["year"] +" "+ obj["major"];

                    if (optionStr === selectedSection) 
                    {
                        sectionId = obj["sectionId"];
                        major = obj["major"];
                    }
                }
            );

            // at this point, sectionId, outcomeId, and major are determined.
            // need to make ajax calls for all pairs of assessment weights and descriptions
            for (let i = 0; i < weights.length; i++)
            {
                // determine correct assessmentId. If assessmentRow already has aId, get it. if not, null.
                if ( $(assessments[i]).hasClass("assInDB") )
                {
                    assessmentId = parseInt( $(assessments[i]).attr("class").split("-").pop() );
                }
                else 
                { assessmentId = "null"; } //BUG: Insert would not work unless null was made a string: "null"

                // get assessment description and weight values
                assessmentDescription = $(descriptions[i]).val();
                weight = parseInt( $(weights[i]).val() );

                assData = {"assessmentId": assessmentId, "sectionId": sectionId, "assessmentDescription": assessmentDescription, "weight": weight, "outcomeId": outcomeId, "major": major};

                // send request to server to inserting/updating assessments
                $.ajax(
                    {
                        url: "php_modules/updateAssessment.php",
                        method: "POST",
                        data: assData,
                        dataType: "json",
                    }
                ).done(
                    function()
                    {
                        //TEST PRINT
                        console.log("insertion of ", assData, " succeeded!");
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

        let deleteAssessments = function()
        {
            let assesToDelete = window.sessionStorage.getItem("deleteAssessment");
            assesToDelete = JSON.parse(assesToDelete);
            let aId = 0;

            if (assesToDelete !== null)
            {
                // send ajax request for deletion for all items in "deleteAssessment"
                assesToDelete.forEach( 
                    // function(index, obj)
                    function(obj, index)
                    {
                        aId = obj["assessmentId"];

                        $.ajax(
                            {
                                url: "php_modules/deleteAssessment.php",
                                method: "POST",
                                data: {"assessmentId": aId},
                                dataType: "json",
                            }
                        ).done(
                            function()
                            {
                                //TEST PRINT
                                console.log("deletion of ", aId, " succeeded!");
                            }
                        ).fail(
                            function(jqXHR)
                            {
                                $("#loginMsg").html("There was a problem contacting the server: <br>" +
                                jqXHR.status + " " + jqXHR.responseText);
                            }
                        );
                    }
                );

                // all assessments should be deleted. Remove them.
                window.sessionStorage.removeItem("deleteAssessment");
            }
        }

        // update assessments in database
        removeModifiedFlags();
        deleteAssessments();
        updateAssessments();
    }

    // ERROR CHECKING ---------------------------------------------------------------------------------------
    resetWtsDescrsStyles();
    descrBlankChk();
    wtBlankChk();

    let sumNot100 = !wtSumChk();
    let blanksFound = ( blankDescriptions.length || blankWeights.length );
    let wtOutOfBounds = wtBoundsChk();
    let errMsgFullyVisible = !assError.is(":animated") && (currHeight === maxHeightInt); //not animated AND at max height
    let errMsgFullyNOTvisible = !assError.is(":animated") && (currHeight === minHeightInt); //not animated AND at min height
    let errMsgCurrentlyAnimated = assError.is(":animated");

// NOTE: bracket here only to allow all these if's/else's to be collapsed
{    
    if ( sumNot100 && blanksFound && wtOutOfBounds) 
    {
        assError.html("assessment weights must add to 100<br>assessment descriptions and weights must not be blank<br>assessment weights must be between 1 and 100"); 
        assError.css("color", "red");
        highLightBlanks();
        highLightOutOfBounds();
    }
    else if ( sumNot100 && wtOutOfBounds) 
    {
        assError.html("assessment weights must add to 100<br>assessment weights must be between 1 and 100"); 
        assError.css("color", "red");
        highLightBlanks();
        highLightOutOfBounds();
    }
    else if ( sumNot100 && blanksFound) 
    {
        assError.html("assessment weights must add to 100<br>assessment descriptions and weights must not be blank"); 
        assError.css("color", "red");
        highLightBlanks();
    }
    else if ( blanksFound && wtOutOfBounds) 
    {
        assError.html("assessment descriptions and weights must not be blank<br>assessment weights must be between 1 and 100"); 
        assError.css("color", "red");
        highLightBlanks();
        highLightOutOfBounds();
    }
    else if ( sumNot100 ) 
    { 
        assError.html("assessment weights must add to 100"); 
        assError.css("color", "red");
    }
    else if ( blanksFound )  // non-zero length evalutes to true
    {
        assError.html("assessment descriptions and weights must not be blank"); 
        assError.css("color", "red");
        highLightBlanks();
    }
    else if (wtOutOfBounds) 
    {
        assError.html("assessment weights must be between 1 and 100"); 
        assError.css("color", "red");
        highLightOutOfBounds();
    }
    else
    {
        assError.html("assessment saved successfully!"); 
        assError.css("color", "rgb(87, 186, 88)");
    }
}
    // ANIMIATIONS AND SUBMISSION---------------------------------------------------------------------------------------
    if ( errMsgCurrentlyAnimated ) {} // error msg is currently animated. Do NOTHING.

    else if ( !sumNot100 && !blanksFound && !wtOutOfBounds && errMsgFullyNOTvisible) // no errors found, error msg not visible
    {
        assError.animate( {height: maxHeight}, animationSpeed, // animate error message appearing
            function() // animate error message disappearing
            {
                assError
                .delay(animationSpeed * 6)
                .animate({height: minHeight}, animationSpeed, 
                    function() //call back after animation is complete
                    {
                        assError.html("");
                    }
                );
            }    
        );

        // all clear. update and delete assessments
        updateDeleteAssessments();
    }

    else if ( (sumNot100 || blanksFound || wtOutOfBounds) && errMsgFullyNOTvisible ) // errors found, error msg not visible. 
    {
        assError.animate({height: maxHeight}, animationSpeed); // animate error message appearing
    }

    else if ( (sumNot100 || blanksFound || wtOutOfBounds) && errMsgFullyVisible) // errors found, error msg visible
    { /* do nothing. error message needs to stay visible which it is already. */   }

    else // no errors found, error msg visible 
    {
        assError
        .delay(animationSpeed * 6)
        .animate({height: minHeight}, animationSpeed, 
            function() //call back after animation is complete
            {
                assError.html("");
            }
        );

        // all clear. update and delete assessments
        updateDeleteAssessments();
    }

}

// register assessment check/ 
saveAss.click( assChecksAndSubmission );