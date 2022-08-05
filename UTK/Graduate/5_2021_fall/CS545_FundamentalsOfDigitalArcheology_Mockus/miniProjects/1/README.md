Mini Project1 - Text Analysis
===========

Please read the entire assignment.

Workflow
===============
1. To start, [**fork** repository][forking] [github.com/fdac21/Miniproject1][assignment]
    1. Go to github.com/fdac21/Miniproject1
    1. Click on "Fork Repository" button
	1. Select your GitHubID as the organization to fork to
	1. Go to Settings and enable issue tracking
1. Connect to your docker container
1. [**Clone**][ref-clone] the forked repository to your docker container

     ```
     git clone github:YourGHUsername/Miniproject1
     ```
1. Copy [Miniproject1.ipynb](https://github.com/fdac21/Miniproject1/blob/master/Miniproject1.ipynb) to YourUTKID.ipynb

       ```
       git add YourUTKID.ipynb
       git commit -m "YOUR commit message"
       ```
1. Now back in the shell [**Push**][ref-push]/sync the changes to github.

	git push

1. At https://github.com/YourGHUsername/Miniproject1
   Create a [**pull request**][pull-request] on the
   original repository [fdac21/Miniproject1][assignment]  to
   turn in the assignment.


Social Workflow with deadlines
==============================

1. Do the first four steps listed above before the class on Sep 9: if you have any
   issues, we need to clarify them before Sep 9.
1. Come up with an idea of what analysis you are going to use and
   what data sources you will use. Discuss it with your assigned peer on Sep 9.
   You present the idea to your peer next to you in the list of groups and you
   produce a comment for the peer before you in the group list below. 
	 
   The first person (e.g., dlomax)  comments on the presentation of
   the last person (hkitts2) and the last person presents to first.  To access your peer's forked repository, manually enter the url:
   https://github.com/YourPeersGHUsername/Miniproject1
   The mapping of the [GH ids to UTK IDs](https://github.com/fdac21/students/blob/master/ports.md) 

When you raise an issue, you should assign it to the GHUSER who should comment on it.
1. Start doing the analysis and discuss your progress with your peer
   so that you are done before class on Sep 14. In case you encounter any problems
   retrieving, storing, or analyzing data please discuss them with
   your peer and, if needed, escalate.
1. Prepare a few slides (or a jupyter notebook) explaining the approach and findings and
   present to a small group as specified below.
1. We will have the entire class on Sep 14 for you to practice presentations in several groups

### Groups

1. dlomax jhammer2 wsquires kleland asuther8 hkitts2 
1. spatel91 dschult9 mjahansh tainley rbhupati vaiely 
1. rbarklow shwang6 azou fxie6 abanks19 gbrown39 
1. kcavitt psherri1 pvelas ahughe51 jmuncy2 amalviya 
1. jlemon3 mmohandi showel17 avo5 mxw557 jnotowic 
1. tnguye85 wcoar ahende39 rbray2 jhills mdixson2 
1. wjohns53 bmingled dtroutm1 bperosa jhamil53 mmuszik1 
1. jmatloc1 rwill148 cukaegbu yli53 mrm634 dmaioran 
1. jmoczadl tflorey xcao14 rrusitan sxue3 ppatel91 
1. kpatel87 rstewa35 bgreenb3 reehsemm lgonzal6 plin5 
1. ciadonis jcarmac3 astjohn1 moliaee2 fxu11 klim7

    
At the end of the presentations within the group a vote will be held (by that group) for one presentation 
to be selected for the presentation to the entire class. The first person listed in each group will record that in the issue 
https://github.com/fdac21/Miniproject1/issues/1


What operational data traces are expected
==
1. I expect you to make at least one commit -not pull request (in your cloned
   repo) by Sep 9, at least three by Sep 14, and at least five total.
1. Each interaction with your peer should result in an issue that
   describes your peers comments. You should raise at least one issue against your peers repository peer/Miniproject1 (see above), and you should make at least one commment on the issue raised against your fork (you/Miniproject1). 

Presentations should include
===
* What is the question?
* What was the approach?
* What problems did I encounter?
* What results did I get?
* What new ideas did this generate?


<!-- Links -->
[assignment]: https://github.com/fdac21/Miniproject1
[forking]: https://guides.github.com/activities/forking/
[pull-request]: https://help.github.com/articles/creating-a-pull-request
[ref-clone]: https://help.github.com/en/articles/cloning-a-repository
[ref-commit]: https://readwrite.com/2013/10/02/github-for-beginners-part-2/
[ref-push]: https://help.github.com/en/articles/pushing-commits-to-a-remote-repository
