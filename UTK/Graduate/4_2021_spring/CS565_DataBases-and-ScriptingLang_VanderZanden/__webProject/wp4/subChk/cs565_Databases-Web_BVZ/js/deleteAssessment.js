function deleteAssessment()
{
    let assToDelete = $(this).parent().parent();
    let assToDelete_classes = assToDelete.attr("class").split(/\s+/); //split based on ANY whitespace
    let assToDelete_assessmentId = 0;
    let assToDelete_weight = assToDelete.find(".assessmentRow-1").val();
    let assToDelete_description = assToDelete.find(".assessmentRow-2").val();
    let asses = $(".assessmentRow"); // array of all currently available assessments.
    
    // returns true if ass is already in server database. false otherwise.
    let isAssInDB = function (assToDelete) 
    {
        if ( assToDelete.attr("class").includes("assInDB") ) {return true;}
        else {return false;}
    };
    
    // stores assessment in sessionStorage with all data needed (probably more) to delete it from DB
    let storeAssForDeletion = function (assToDelete_assessmentId, assToDelete_weight, assToDelete_description)
    {
        //returns null if deleteAssessment doesn't exist
        let assesToDelete = window.sessionStorage.getItem("deleteAssessment"); 

        //no assessments are currently stored for deletion. Start a new array
        if (assesToDelete === null) { assesToDelete = []; }
        else { assesToDelete = JSON.parse(assesToDelete); } // parse JSON string.
                                                            //This converts it to an array of JSON objects!!

        // put new JSON object       
        assesToDelete.push(
            {
                "assessmentId": assToDelete_assessmentId,
                "weight": assToDelete_weight,
                "description": assToDelete_description
            }
        );

        // IMPORTANT: JSON array must be stringified before storing in sessionStorage
        //            If it isn't, JSON.parse() will throw errors. 
        assesToDelete = JSON.stringify(assesToDelete);

        // re-store updated assessments needing deletion
        window.sessionStorage.setItem("deleteAssessment", assesToDelete);
    };   

    // get assessmentID from DOM element
    // assToDelete.attr("class").split(/\s+/).forEach(
    assToDelete_classes.forEach(
        (str) => 
        {
            if (str.includes("aID"))  
            { assToDelete_assessmentId = parseInt( str.split("-").pop() ); }
        } 
    );

    // if assessment being deleted is the last one in the DOM, just clear it. Otherwise, delete it from DOM.
    if (asses.length === 1) //only one assessment remaining. keep it. clear text.
    {
        // returns true if assToDelete is from DB. If true, store it for deletion in sessionStorage
        // also, remove "assInDB" and "aID-#" from class to completely reset element
        if ( isAssInDB(assToDelete) ) 
        { 
            storeAssForDeletion(assToDelete_assessmentId, assToDelete_weight, assToDelete_description); 
            assToDelete.attr("class", "assessmentRow");
        }

        // reset element values
        $(".assessmentRow-1").val(""); // clear weight
        $(".assessmentRow-2").val(""); // clear description
    }
    else 
    { 
        //returns true if assToDelete is from DB. If true, store it for deletion in sessionStorage
        if ( isAssInDB(assToDelete) ) 
        { storeAssForDeletion(assToDelete_assessmentId, assToDelete_weight, assToDelete_description); }
        
        assToDelete.remove(); 
    }

    window.sessionStorage.setItem("pageModified", "yes");
}

// find all trash buttons with class selector
$(".assessmentRow4-3_Btn").click(deleteAssessment);
