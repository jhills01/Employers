using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Burst;
using Unity.Jobs;
//using Unity.Collections;
using UnityEngine.Jobs;
using Unity.Mathematics;


//global game state manager reference
using gsm = GameStateManager;
using km = KnightManager;

public class UpdateManager : MonoBehaviour
{
    ///*
    //declare class variables

    void Update()
    { 
        if ( Input.GetKeyDown(KeyCode.UpArrow) )
        {
            //NOTE: Debug.Log does add some overhead, but I'm still getting FPS stutter when pressing UpArrow
            //Debug.Log("Pressed: yay!");

            //Spawner.SpawnShell();
            //Debug.Log("gsm.numOfKnights:  " + gsm.numOfKnights);

            //NOTE: Need to check if JOB resources have already been allocated. if so, destroy them and start over
            if (gsm.JOBS && !gsm.ECS)
            {
                Spawner.SpawnShell();
                Debug.Log("gsm.numOfKnights:  " + gsm.numOfKnights);

                //Debug.Log("gsm.knights.Count: " + gsm.knightTransforms.Count);
                km.updateKnights();
            }
        }

        if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            //NOTE: Debug.Log does add some overhead, but I'm still getting FPS stutter when pressing UpArrow
            //Debug.Log("Pressed: yay!");
            if (!gsm.JOBS)
            {
                Spawner.SpawnShell();
                //Debug.Log("gsm.knights.Count: " + gsm.knightTransforms.Count);
                Debug.Log("gsm.numOfKnights:  " + gsm.numOfKnights);
            }
        }

        /*
        if ( Input.GetKeyDown(KeyCode.Space) )
        {
            if (gsm.ECS) gsm.ECS = false;
            else gsm.ECS = true;
        }
        */

        if (gsm.JOBS && !gsm.ECS)
        {
            //gsm.changeDirection = 0;
            //--gsm.time;

            //if (gsm.time <= 0.0f)
            //{
            //    gsm.time = gsm.timeInterval;

            //    gsm.randomDirection.x = gsm.directionChoices[gsm.random.NextInt(gsm.directionChoices.Length)];
            //    gsm.randomDirection.z = gsm.directionChoices[gsm.random.NextInt(gsm.directionChoices.Length)];

            //    gsm.changeDirection = 1;
            //}

            //NOTE: put JOB code here
            knightJob job = new knightJob { };
            km.rotationJobHandle = job.Schedule(km.transformAccessArray);
            JobHandle.ScheduleBatchedJobs(); //This manually causes job to immediately execute instead of delay. see "https://docs.unity3d.com/ScriptReference/Unity.Jobs.JobHandle.ScheduleBatchedJobs.html"
        }
    }

    
    private void LateUpdate()
    {
        if (gsm.JOBS && !gsm.ECS) km.rotationJobHandle.Complete();
    }

    /*
    private void OnDisable()
    {
        km.transformAccessArray.Dispose();
    }
    */
}
