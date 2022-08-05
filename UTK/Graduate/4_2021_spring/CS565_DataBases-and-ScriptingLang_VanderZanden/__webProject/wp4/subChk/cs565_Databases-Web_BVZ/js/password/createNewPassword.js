 $(
     $("#changePwRow4_Submit").click(
         function()
         {
            let email = window.sessionStorage.getItem("email");
            let newPassword = $("#input_NewPassword").val();
            let confirmPassword = $("#input_ConfirmPassword").val();
            let $errMsg = $("#errMsg");
            let currHeight = $errMsg.height();
            let maxHeight = "10px";
            let minHeight = "0px";
            let maxHeightInt = 10;
            let minHeightInt = 0;
            let animationSpeed = 500;
            let changePassword = false;
            let errMsgCurrentlyAnimated = $errMsg.is(":animated");

            if (newPassword === "" || confirmPassword === "")
            {   
                $errMsg.css("color", "red");
                $errMsg.text("Passwords fields cannot be blank. Please correct.");
            }
            else if (newPassword !== confirmPassword)
            {
                $errMsg.css("color", "red");
                $errMsg.text("Passwords must match. Please correct.");
            }
            else
            {
                $errMsg.css("color", "rgb(87, 186, 88)");
                $errMsg.text("Password changed!");

                changePassword = true;
            }

            if (currHeight === minHeightInt)
            {
                $errMsg.animate( {height: maxHeight}, animationSpeed);
            }

            // call ajax
            if (changePassword)
            {
                // close error msg
                $errMsg.delay(4*animationSpeed).animate( {height: minHeight}, animationSpeed,
                    function()
                    {
                        $errMsg.text("");
                    }    
                );

                // send request to server to inserting/updating assessments
                $.ajax(
                    {
                        url: "php_modules/updatePassword.php",
                        method: "POST",
                        data: {"email": email, "password": newPassword},
                        dataType: "json"
                    }
                ).done(
                    function()
                    {
                        //TEST PRINT
                        console.log(`insertion of ${newPassword} succeeded!`);
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
    ) 
 );