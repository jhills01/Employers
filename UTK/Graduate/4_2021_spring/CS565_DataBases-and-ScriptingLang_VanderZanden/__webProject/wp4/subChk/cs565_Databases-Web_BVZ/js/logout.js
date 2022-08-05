$( //NOTE: callback for clicking menu button is in "changePassword.js"
    function()
    {
        $("#logout").click(
            function()
            {

                // $("#subH2_DropDownBtn_Content").css("display", "none");

                // navigate to password.php
                window.location = "login.html";
            }
        );
    }
);