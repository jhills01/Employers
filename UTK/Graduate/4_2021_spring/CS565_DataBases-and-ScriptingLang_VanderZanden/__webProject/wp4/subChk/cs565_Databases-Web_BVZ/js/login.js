//NOTE: you have to use FORM element, not the submit button element 
$("#loginForm").submit(
    function ()
    {
        //prevents default behavior for form getting submitted. 
        event.preventDefault();

        let $email = $("#email").val();
        let $password = $("#password").val();


        // check email/password in database and go to abet.php if valid
        $.ajax(
            {
                url: "php_modules/login.php",
                method: "POST",
                data: {email: $email, password: $password},
                dataType: "json",
            }
        ).done(
            function(data)
            {
                //no data returned b/c email/password was invalid
                if (data.length == 0)
                {
                    // change loginMsg to show email/pw error
                    $("#loginMsg").html("invalid e-mail or password")
                }

                //store returned data in clent-side sessionStorage array and load abet.php 
                // IMPORTANT NOTE: it's more efficient to store data in _SESSION array in PHP on the server side!!!!!
                else
                {
                    // change loginMsg to show nothing
                    $("#loginMsg").html("")

                    // store session variables
                    // NOTE: JSON.stringify is needed to convert [object Object, object Object, ...] into string representing... 
                    //       ... correct data. 
                    data = JSON.stringify(data);
                    // data = JSON.parse(data);
                    window.sessionStorage.clear(); //make sure we start abet.php with clean slate
                    
                    window.sessionStorage.setItem("login", data);
                    window.sessionStorage.setItem("email", $email);

                    window.location = "abet.php";
                }
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