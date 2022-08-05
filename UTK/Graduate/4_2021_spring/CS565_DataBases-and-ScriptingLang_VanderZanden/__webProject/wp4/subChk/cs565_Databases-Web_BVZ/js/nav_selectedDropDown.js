// console.log("STARTING selectedDropDown SCRIPT");

let selected = $("#nav_DropDown").find(":selected").text();
let lastSelected = window.sessionStorage.getItem("lastSelectedDropDown");

// console.log("selected: ", selected, " --- lastSelected: ", lastSelected);

if (lastSelected === null) 
{
    window.sessionStorage.setItem("lastSelectedDropDown", selected);
    // window.location.href="abet.php?selectedOption=" + selected;
}
else if (selected !== lastSelected) 
{
    // console.log("selected !== lastSelected");
    $("#nav_DropDown").val(lastSelected); //changes automatically selected option after refresh back to lastSelected option from sesionStorage.

    // window.location.href="abet.php?selectedDropDown=" + lastSelected; //THIS REFRESHES THE PAGE RUNNING ALL PHP CODE AGAIN!!!

}

// console.log("EXITING selectedDropDown SCRIPT");

//callback for when I change selected option
$("#nav_DropDown").change(
    function()
    {
        // console.log("STARTING selectedDropDown CALLBACK");

        let selected = $("#nav_DropDown").find(":selected").text();
        let lastSelected = window.sessionStorage.getItem("lastSelectedDropDown");
        
        //reset next selected section's outcome to 1st
        let firstOutcome = $("#nav_Outcomes a").first();  
        let previouslySelectedOutcome = $(".selectedOutcome");

        // console.log("selected: ", selected, " --- lastSelected: ", lastSelected);

        if (lastSelected === null) 
        {
            window.sessionStorage.setItem("lastSelectedDropDown", selected);
            // window.location.href="abet.php?selectedOption=" + selected;
        }
        // else if (selected === lastSelected) { /*Do nothing b/c we haven't changed anything*/ }
        // else if (selected !== lastSelected)
        else
        { 
            // when changing outcomes, deleteAssessment in sessionStorage needs to be deleted
            // ...so we can start fresh
            window.sessionStorage.removeItem("deleteAssessment");

            window.sessionStorage.setItem("lastSelectedDropDown", selected);
            
            //reset next selected section's outcome to 1st
            window.sessionStorage.setItem("previouslySelectedOutcome", previouslySelectedOutcome.text());
            window.sessionStorage.setItem("currentlySelectedOutcome", firstOutcome.text());

            window.location.href=`abet.php?selectedOutcome=${firstOutcome.text()}&selectedDropDown=${selected}`;
            
            // window.location.href="abet.php?selectedDropDown=" + selected; //THIS REFRESHES THE PAGE RUNNING ALL PHP CODE AGAIN!!!
        }

        // console.log("EXITING selectedDropDown CALLBACK");
    }
);