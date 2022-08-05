$(
    function()
    {
        let selectedSection = window.sessionStorage.getItem("lastSelectedDropDown");
        let selectedOutcome = window.sessionStorage.getItem("currentlySelectedOutcome");
        // let outcomeId = parseInt( selectedOutcome.split(/\s+/).pop() );
        let outcomeId = selectedOutcome.split(/\s+/).pop();
        
        $("#nav_DropDown").val(selectedSection);
        
        
    }
);