$("#assRow5_btn").click(
    function()
    {
        //new element string
        let newAss =  
        "<div class=\"assessmentRow\"> " +
            "<input class=\"assessmentRow-1\" type=\"number\" min=1 max=100> " +
            "<textarea class=\"assessmentRow-2\" rows=\"4\" maxlength=\"400\"></textarea> " +
            "<div class=\"assessmentRow-3\"> " +
                "<button class=\"assessmentRow4-3_Btn\" type=\"button\"> " +
                "<!-- Made this icon a class b/c there is more than one --> " +
                    "<img class=\"icon_trash\" src=\"icons/trash.svg\"> " +
                "</button> " +
            "</div> " +
        "</div>";
    
        // get all assessmentRows in an array
        let allAss = $(".assessmentRow");

        // attach new assessment AFTER the LAST assessment in array
        // let latestAddedAss = allAss.last().after(newAss);
        allAss.last().after(newAss);
        allAss = $(".assessmentRow");

        // register click callback with newly added trash button
        //NOTE: deleteAssessment is defined in deleteAssessment.js
        allAss.last().find(".assessmentRow4-3_Btn").click(deleteAssessment);

        // allAss.last().find(".assessmentRow4-3_Btn").click(
        //     function ()
        //     {
        //         let assToDelete = $(this).parent().parent();
        //         let ass = $(".assessmentRow");
                
        //         if (ass.length === 1) //only one assessment remaining. keep it. clear text.
        //         {
        //             $(".assessmentRow-1").val(""); 
        //             $(".assessmentRow-2").val("");
        //         }
        //         else { assToDelete.remove(); }
        //     }
        // );
    }
);
