//NOTE: "defer" attribute only works with external scripts with src attribute
// console.log("STARTING selectedOutcome SCRIPT");

let previouslySelectedOutcome = window.sessionStorage.getItem("previouslySelectedOutcome");
let currentlySelectedOutcome = window.sessionStorage.getItem("currentlySelectedOutcome");


// console.log("previouslySelectedOutcome: ", previouslySelectedOutcome);
// console.log("currentlySelectedOutcome: ", currentlySelectedOutcome);

let outcomes = $("#nav_Outcomes a");

let outcomeFound = function (outcome)
{
    let match = false;

    if ( !outcome ) {return true;}

    outcomes.each(
        function(index, obj)
        {
// console.log("obj: ", $(obj).text());          
// console.log("outcome: ", outcome.text());            
            if ($(obj).text() === outcome) { match = true;}
        }
    );

    return match;
}


let selectedStyle = 
{
    "background-color": "rgb(200, 200, 200)",
    "outline": "none"
};
let unselectedStyle =
{
    "background-color": "rgb(233, 233, 233)",
    "outline": "none"
};

// add "outcome" class to all outcomes under "nav_Outcomes" element
// then, add "hover" coloring callbacks
$("#nav_Outcomes a").addClass("outcome");
$(".outcome").mouseenter(
    function ()
    {
        let classes = $(this).attr("class");

        if ( classes.includes("selectedOutcome") ) {/*do nothing. coloring of selected outcome already done */}
        else { $(this).css("background-color", "rgb(200, 200, 200)"); }
    }
).mouseleave(
    function ()
    {
        let classes = $(this).attr("class");

        if ( classes.includes("selectedOutcome") ) {/*do nothing. coloring of selected outcome already done */}
        else { $(this).css("background-color", "rgb(233, 233, 233)"); }
        
    }
);


// if last is null, we just loaded the page for the first time. Otherwise, get currently selected outcome
if (previouslySelectedOutcome === null) 
{
// console.log("previouslySelectedOutcome === null");
    let firstSelectedOutcome = outcomes.first();
    
    firstSelectedOutcome.css(selectedStyle);
    firstSelectedOutcome.addClass("selectedOutcome");

    window.sessionStorage.setItem("previouslySelectedOutcome", firstSelectedOutcome.text());
    window.sessionStorage.setItem("currentlySelectedOutcome", firstSelectedOutcome.text());

    // establishes selectedOutcome value in GET for php
    window.location.href="abet.php?selectedOutcome=" + firstSelectedOutcome.text(); //THIS REFRESHES THE PAGE RUNNING ALL PHP CODE AGAIN!!!
}
else if (previouslySelectedOutcome !== currentlySelectedOutcome)
// else
{
// console.log("previouslySelectedOutcome !== currentlySelectedOutcome");
// console.log(previouslySelectedOutcome previouslySelectedOutcome)
// console.log(previouslySelectedOutcome previouslySelectedOutcome)

    // contains returns text of element
    previouslySelectedOutcome = $( `#nav_Outcomes a:contains(${previouslySelectedOutcome})` );
    previouslySelectedOutcome.css(unselectedStyle);
    previouslySelectedOutcome.removeClass("selectedOutcome");

    currentlySelectedOutcome = $( `#nav_Outcomes a:contains(${currentlySelectedOutcome})` );

    // assuming false if the string for contains wasn't found
// if ( !outcomeFound(currentlySelectedOutcome.text()) ) { currentlySelectedOutcome = outcomes.first() }

    currentlySelectedOutcome.css(selectedStyle);
    currentlySelectedOutcome.addClass("selectedOutcome");


    window.sessionStorage.setItem("previouslySelectedOutcome", currentlySelectedOutcome.text());
    window.sessionStorage.setItem("currentlySelectedOutcome", currentlySelectedOutcome.text());
}
else if (previouslySelectedOutcome === currentlySelectedOutcome)
{
// console.log("previouslySelectedOutcome === currentlySelectedOutcome");

    // contains returns text of element
    currentlySelectedOutcome = $( `#nav_Outcomes a:contains(${currentlySelectedOutcome})` );
    
// assuming false if the string for contains wasn't found
// if (!outcomeFound(currentlySelectedOutcome)) { currentlySelectedOutcome = outcomes.first() }

    currentlySelectedOutcome.css(selectedStyle);
    currentlySelectedOutcome.addClass("selectedOutcome");

    window.sessionStorage.setItem("previouslySelectedOutcome", currentlySelectedOutcome.text());
    window.sessionStorage.setItem("currentlySelectedOutcome", currentlySelectedOutcome.text());

}

//setup callbacks on all outcome children
outcomes.click(
    function()
    {
// console.log("entered click");        
        // get selected outcome info
        let previouslySelectedOutcome = $(".selectedOutcome");
        let currentlySelectedOutcome = $(this); //get item that was clicked/selected
        // get selected section info
        let selectedSection = $("#nav_DropDown").find(":selected").text();

// console.log("previouslySelectedOutcome: ", previouslySelectedOutcome);
// console.log("currentlySelectedOutcome: ", currentlySelectedOutcome);
// console.log("selectedSection: ", selectedSection);

        window.sessionStorage.setItem("previouslySelectedOutcome", previouslySelectedOutcome.text());
        window.sessionStorage.setItem("currentlySelectedOutcome", currentlySelectedOutcome.text());
                
        if ( previouslySelectedOutcome.text() === currentlySelectedOutcome.text() ) 
        { /*DO NOTHING*/ }
        else 
        {
            // when changing outcomes, deleteAssessment in sessionStorage needs to be deleted
            // ...so we can start fresh
            window.sessionStorage.removeItem("deleteAssessment");

            // send both currently selected outcome AND section in request 
            // instead of just selected outcome. This way, PHP code is AlWAYS 
            // updated with the complete and current state of the site
            // NOTE: this approach solved a big problem. It's probably best send everything
            //       on ALL CALLBACKS regardless!!!
            window.sessionStorage.setItem("lastSelectedDropDown", selectedSection);
            window.location.href=`abet.php?selectedOutcome=${currentlySelectedOutcome.text()}&selectedDropDown=${selectedSection}`;
        }
    }
);


// console.log("EXITING selectedOutcome SCRIPT");

