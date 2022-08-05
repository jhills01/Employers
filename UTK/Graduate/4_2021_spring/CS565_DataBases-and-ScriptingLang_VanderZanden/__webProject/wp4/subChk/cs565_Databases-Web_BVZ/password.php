<!DOCTYPE html>
<html>
    <head>
        <title>UTK ABET - Change Password</title>
        <link rel="stylesheet" href="css/password.css" />
        <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
        <script type="text/javascript" src="js/pageReload.js"></script> 
        <script type="text/javascript" src="js/nav_selectedOutcome.js" defer></script>
        <script type="text/javascript" src="js/nav_selectedDropDown.js" defer></script>
        <script type="text/javascript" src="js/preventEnterKeyRefresh-BtnSubmit.js" defer></script>
        <script type="text/javascript" src="js/password/returnToAbet.js" defer></script>
        <script type="text/javascript" src="js/changePassword.js" defer></script>
        <script type="text/javascript" src="js/logout.js" defer></script>
        <script type="text/javascript" src="js/password/createNewPassword.js" defer></script>

    </head>

    <body>
        <!--header-------------------------------------------------------------------------------->        
        <?php include "header.php";?>

        <!--nav-------------------------------------------------------------------------------->
        <?php include "nav.php";?>

        <!--changePw-------------------------------------------------------------------------------->
        <div class="changePw" id="changePw">
            <div id="changePwRow1">Change Password</div>         
            
            <div id=changePwRow2>
                <div id="changePwRow2_BasicInfo">Basic Info</div>
                
                <div id="changePwRow2_NameEmail">
                    <div id="name">
                        <label for="input_Name">Name:</label>
                        <input type="text" id="input_Name" value="Wily Coyote" size="100" readonly>
                    </div>
                    <div id="email">    
                        <label for="input_Email">Email:</label>
                        <input type="text" id="input_Email" value="coyote@utk.edu" size="100" readonly>
                    </div>
                </div>
            </div>

            <div id="changePwRow3">
                <div id="changePwRow3_ChangePW">Change Password</div>
                
                <!-- <div id=newConfirmSubmitPW> -->
                <form id=newConfirmSubmitPW>
                    <div id="changePwRow3_NewPassword">
                        <label for="input_NewPassword">New Password</label> <br>
                        <input type="password" id="input_NewPassword" maxlength="20">
                    </div>
                    
                    <div id="changePwRow3_ConfirmPassword">
                        <label for="input_ConfirmPassword">Confirm Password</label> <br>
                        <input type="password" id="input_ConfirmPassword" maxlength="20">
                    </div>

                    <div id="changePwRow4">
                        <button type="submit" id="changePwRow4_Submit">Submit</button>
                        <!-- <span id="errMsg"></span> -->
                        <p id="errMsg" style="height: 0px"></p>
                    </div>
                </form>
            </div>
        </div>

        <!--footer------------------------------------------------------------------------------>
        <div class="footer1" id="footer1"></div>
        <div class="footer2" id="footer2"></div>
        <div class="footer3" id="footer3"></div>
    </body>
</html>