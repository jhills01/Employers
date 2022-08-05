$(
    function()
    {
        let $outcomes = $("#nav_Outcomes a");

        $outcomes.click(
            function()
            {
                let abetReturnURL = window.sessionStorage.getItem("abet_href");
                window.location.href = abetReturnURL;
            }

        );
    }
);