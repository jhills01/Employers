//using System.Collections; //don't know if I need these
//using System.Collections.Generic; //don't know if I need these 
using UnityEngine;
using UnityEngine.Jobs;
using Unity.Collections;
using Unity.Jobs;
using Unity.Burst;
using Unity.Mathematics;

using gsm = GameStateManager;
using UnityEngine.UIElements;
//using System;

//[BurstCompile(FloatPrecision.Low, FloatMode.Fast)]
[BurstCompile]
public struct knightJob : IJobParallelForTransform
{
    public void Execute(int index, TransformAccess transform)
    {

        //NOTE: Might not need this. Depends on how Entities work with IJobParallelForTransform
        //if (!gsm.ECS)
        //{
        transform.rotation = Quaternion.Euler(0, 0.2f, 0) * transform.rotation;

        float minX = gsm.knightStartPosList[index].x - 2.0f;
        float maxX = gsm.knightStartPosList[index].x + 2.0f;
        
        float minZ;
        float maxZ;

        if (gsm.knightIDList[index] == 0f)
        {
            minZ = gsm.knightStartPosList[index].z - 1.0f;
            maxZ = gsm.knightStartPosList[index].z + 3.0f;
        }
        else
        {
            minZ = gsm.knightStartPosList[index].z - 3.0f;
            maxZ = gsm.knightStartPosList[index].z + 1.0f;
        }

        Vector3 speed = new Vector3(0.01f, 0.0f, 0.01f);
        Vector3 direction = gsm.knightDirectionList[index];

        //check if direction needs to be randomly changed
        //if (gsm.changeDirection == 1)
        //{
        //    direction.x *= gsm.randomDirection.x;
        //    direction.z *= gsm.randomDirection.z;
        //}

        transform.position += new Vector3(speed.x * direction.x, 0.0f, speed.z * direction.z);

        //checks if nextPos.x is out of bounds. If so, x direction is reversed
        if (transform.position.x <= minX || transform.position.x >= maxX)
        {
            direction.x *= -1.0f;
            gsm.knightDirectionList[index] = direction;
        }
        //checks if nextPos.z is out of bounds. If so, z direction is reversed
        if (transform.position.z <= minZ || transform.position.z >= maxZ)
        {
            direction.z *= -1.0f;
            gsm.knightDirectionList[index] = direction;
        }

        transform.position += new Vector3(speed.x * direction.x, 0.0f, speed.z * direction.z);

        //added this for increased computation time per knight
        if (gsm.killme == 1)
        {
            float value = 0f;
            for (int i = 0; i < 50000; ++i)
            {
                value = math.exp10(math.sqrt(value));
            }
        }
    }

    /* 
    internal static JobHandle Schedule(TransformAccessArray transformAccessArray)
    {
        throw new NotImplementedException();
    }
    */
}

public class KnightManager : MonoBehaviour
{
    //JOB DATA FOR ROTATION
    //private NativeArray<float> rotationData;
    public static JobHandle rotationJobHandle;
    public static TransformAccessArray transformAccessArray;
    public static Transform[] knightTransforms;

    public static void initializeKnights ()
    {
        //NOTE: I had to do IF-ELSE b/c gsm.knightTransforms is setup with gameObject transforms in GameStateManager. I need to replace it with entity transforms. 
        //...but the problem with that is that I don't have any entities when scenes starts unlike gameObjects where I have two already there
        if (gsm.JOBS && !gsm.ECS)
        {
            //if (!transformAccessArray.isCreated) { Debug.Log("TAarr.isCreated return FALSE before calling NEW!"); /*transformAccessArray.Dispose()*/; }
            knightTransforms = gsm.knightTransforms.ToArray(); //automatically converts LIST to ARRAY --- yay!!!
            transformAccessArray = new TransformAccessArray(knightTransforms);

            //gsm.knightSpeedArr0 = gsm.knightSpeedList0.ToArray();
            //gsm.knightDirectionArr0 = gsm.knightDirectionList0.ToArray();
            //gsm.knightSpeedArr0 = gsm.knightSpeedList0.ToArray();
            //gsm.knightDirectionArr0 = gsm.knightDirectionList0.ToArray();

            //if (transformAccessArray.isCreated) { Debug.Log("TAarr.isCreated return TRUE after calling NEW!"); }
        }
        else
        {

        }
    }

    public static void updateKnights()
    {
        if (gsm.JOBS && !gsm.ECS)
        {
            //if (!transformAccessArray.isCreated) { Debug.Log("TAarr.isCreated return FALSE before calling NEW!"); /*transformAccessArray.Dispose()*/; }
            transformAccessArray.Dispose();

            knightTransforms = gsm.knightTransforms.ToArray(); //automatically converts LIST to ARRAY --- yay!!!
            transformAccessArray = new TransformAccessArray(knightTransforms);

            //gsm.knightSpeedArr0 = gsm.knightSpeedList0.ToArray();
            //gsm.knightDirectionArr0 = gsm.knightDirectionList0.ToArray();
            //gsm.knightSpeedArr0 = gsm.knightSpeedList0.ToArray();
            //gsm.knightDirectionArr0 = gsm.knightDirectionList0.ToArray();

            //if (transformAccessArray.isCreated) { Debug.Log("TAarr.isCreated return TRUE after calling NEW!"); }
        }
        else
        {

        }
    }
}
