// THIS SCRIPT CALLS ALL THINGS THAT MUST OCCUR UPON PAGE RELOAD/REFESH

// delete "deleteAssessment" item in sessionStoage
// this is done b/c PHP reloads from the database anyway on reload/refresh.
// ...No sense in keeping outdated data.
$(  function(){ window.sessionStorage.removeItem("deleteAssessment"); }  );
$(  function(){ window.sessionStorage.removeItem("pageModified"); }  );