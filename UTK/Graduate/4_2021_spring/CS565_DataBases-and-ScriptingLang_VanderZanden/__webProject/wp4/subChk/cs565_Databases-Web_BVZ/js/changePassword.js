$(
    function()
    {
        $("#subH2_DropDownBtn").click(
            function()
            {
                $("#subH2_DropDownBtn_Content").css("display", "block");
            }
        );
    }
);

$(
    function()
    {
        $("#changePassword").click(
            function()
            {

                // $("#subH2_DropDownBtn_Content").css("display", "none");

                // get current state of URL before navigating to password.php
                let abet_href = window.location.href;
                let queryString = abet_href.split("?").pop();

                window.sessionStorage.setItem("abet_href", abet_href);

                // navigate to password.php with query string from abet.php
                window.location.href = `password.php?${queryString}`;
            }
        );
    }
);