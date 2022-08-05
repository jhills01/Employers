// This prevents page refresh when hitting enter key in any input[type=number] field
// However, it also prevents buttons with type=submit
$("form").submit(
    function(key, value)
    {
        event.preventDefault();
    }
);

// $("form").each(
//         function(key, value)
//         {
//             console.log("key",key , "value",value );
//         }
//     );

// $("input[type=number]").bind('keyup', 
//     function(event)
//     { 
//         if(event.keyCode == 13)
//         { 
//             event.preventDefault();
//             // $("#buttonSrch").click(); 
//             return false;
//         }
//     }
// );

// $("input[type=number]").each(
//     function(key, value)
//     {
//         console.log("key",key , "value",value );
//     }
// );
