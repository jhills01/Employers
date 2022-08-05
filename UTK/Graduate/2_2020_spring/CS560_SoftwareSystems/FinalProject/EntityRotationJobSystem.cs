using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Entities;
using Unity.Transforms;
using Unity.Mathematics;
using Unity.Jobs;
using Unity.Burst;

using gsm = GameStateManager;
using km = KnightManager;
using UnityEngine.Experimental.XR;
using Unity.Collections;
using System;

//public class EntityRotationJobSystem : JobComponentSystem, IJobParallelFor
//public class EntityRotationJobSystem : SystemBase
class EntityRotationJobSystem : SystemBase
{
    public readonly float[] directionChoices = { 1.0f, -1.0f };
    public static float timeInterval = 600f; 
    public static float time = timeInterval;

    // Update is called once per frame
    protected override void OnUpdate()
    {
        if (gsm.JOBS && gsm.ECS)
        {
            //All variables that need to be used directly in ForEach must be declared here to avoid errors.
            //Also, you can't write to these variables or you'll get an error. Only use them.
            int changeDirection = 0; //NOTE: Don't need to assign to this in ForEach b/c it is initalized every time!! what a waste!
            Vector3 randomDirection = new Vector3(1.0f, 0.0f, -1.0f);

            //decrease time and check if ZERO is reached
            --time;
            if (time <= 0.0f)
            {
                time = timeInterval;

                randomDirection.x = directionChoices[gsm.random.NextInt(directionChoices.Length)];
                randomDirection.z = directionChoices[gsm.random.NextInt(directionChoices.Length)];

                changeDirection = 1;
            }

            //NOTE: .ScheduleParallel() [at the end of ForEach] only parallizes CHUNKS!!!
            Entities.
                //ForEach((ref Rotation r, ref NonUniformScale nus, ref Translation t, ref Velocity v, ref Boundary b, ref eTime time, ref eEntity e) =>
                ForEach((Entity ent, ref Rotation r, ref Translation t, ref Velocity v, ref Boundary b, in eEntity e, in NonUniformScale nus) =>
                {
                    //Isolates cube entities
                    //"nus.Value.x == 0.25f" check is to ensure that only 1cubes rotate. Not desert squares. 
                    if (nus.Value.x == 0.25f) //could be y or z too
                    {
//if (ent.Index == 8) Debug.Log("ent.Index == 8 --> v.direction: " + v.direction);

                        //ROTATION--------------------------------------------------------------------------------------
                        //NOTE: should I rotate BEFORE or AFTER translation processing????????
                        r.Value = Quaternion.Euler(0, 0.2f, 0) * r.Value;

                        //TRANSLATION--------------------------------------------------------------------------------------
                        //get XZ boundaries for this cube
                        //NOTE: would be better to get this values ONCE, but we're test performance so it's ok. Just make sure you do the same for gameObjects

                        b.minX = b.startPos.x - 2.0f;
                        b.maxX = b.startPos.x + 2.0f;

                        //knight1Entity
                        if (e.id == 1.0f)
                        {
                            b.minZ = b.startPos.z - 3.0f;
                            b.maxZ = b.startPos.z + 1.0f;
                        }
                        //knightEntity
                        else if (e.id == 0.0f)
                        {
                            b.minZ = b.startPos.z - 1.0f;
                            b.maxZ = b.startPos.z + 3.0f;
                        }

                        //check if direction needs to be randomly changed
                        if (changeDirection == 1)
                        {
                            v.direction.x *= randomDirection.x;
                            v.direction.z *= randomDirection.z;
                        }

                        //set new XZ positions
                        t.Value.x += (v.speed.x * v.direction.x);
                        t.Value.z += (v.speed.z * v.direction.z);

                        //checks if nextPos.x is out of bounds. If so, x direction is reversed
                        if (t.Value.x <= b.minX || t.Value.x >= b.maxX) { v.direction.x *= -1.0f; }
                        //checks if nextPos.z is out of bounds. If so, z direction is reversed
                        if (t.Value.z <= b.minZ || t.Value.z >= b.maxZ) { v.direction.z *= -1.0f; }

                        //NOTE: THIS WILL NOT WORK!!! Can't find a way to reference just ONE entity from another!!!!!
                        //check if collision with partner cube has occurred
                        //float radius = Math.Sqrt( Math.Abs(t.Value.x - EntityManager. e.Translation.x ) + () );
                        //Entity partner = e.partner;
                        //var pTranslation = EntityManager.GetComponentData<Translation>(partner);
                        //var pt = GetComponent<Translation>(e.partner);
                        //var pt = GetComponentDataFromEntity<Translation>(partner);

                        //re-set new XZ positions in case there was a change in direction
                        t.Value.x += (v.speed.x * v.direction.x);
                        t.Value.z += (v.speed.z * v.direction.z);

                        ///*
                        ////added this for increased computation time per knight
                        //if (gsm.killme == 1)
                        {
                            //float value = 0f;
                            //for (int i = 0; i < 50000; ++i)
                            //{
                            //    value = math.exp10(math.sqrt(value));
                            //}
                        }
                        //*/
                    }
                })
                .ScheduleParallel();
                //.Schedule();
                //.Run();
                //NOTE: These all seem to be doing the same thing: Multithreading on the worker threads
        }

        /*
        else if (!gsm.JOBS && gsm.ECS)
        {
            //All variables that need to be used directly in ForEach must be declared here to avoid errors.
            //Also, you can't write to these variables or you'll get an error. Only use them.
            int changeDirection = 0; //NOTE: Don't need to assign to this in ForEach b/c it is initalized every time!! what a waste!
            Vector3 randomDirection = new Vector3(1.0f, 0.0f, -1.0f);

            //decrease time and check if ZERO is reached
            --time;
            if (time <= 0.0f)
            {
                time = timeInterval;

                randomDirection.x = directionChoices[gsm.random.NextInt(directionChoices.Length)];
                randomDirection.z = directionChoices[gsm.random.NextInt(directionChoices.Length)];

                changeDirection = 1;
            }

            //Entities.ForEach((Entity e, ref Rotation rotation) =>
            //Entities.ForEach((Entity ent, ref Rotation r, ref Translation t, ref Velocity v, ref Boundary b, in eEntity e, in NonUniformScale nus) =>
            Entities.ForEach((Entity ent, ref Rotation r, ref Translation t, ref Velocity v, ref Boundary b, ref eEntity e, ref NonUniformScale nus) =>
            //Entities.ForEach((ref Rotation rotation) =>
            {
                if ((EntityManager.GetName(ent).ToLower()).Contains("knight"))
                {
                    //ROTATION--------------------------------------------------------------------------------------
                    //NOTE: should I rotate BEFORE or AFTER translation processing????????
                    r.Value = Quaternion.Euler(0, 0.2f, 0) * r.Value;

                    //TRANSLATION--------------------------------------------------------------------------------------
                    //get XZ boundaries for this cube
                    //NOTE: would be better to get this values ONCE, but we're test performance so it's ok. Just make sure you do the same for gameObjects

                    b.minX = b.startPos.x - 2.0f;
                    b.maxX = b.startPos.x + 2.0f;

                    //knight1Entity
                    if (e.id == 1.0f)
                    {
                        b.minZ = b.startPos.z - 3.0f;
                        b.maxZ = b.startPos.z + 1.0f;
                    }
                    //knightEntity
                    else if (e.id == 0.0f)
                    {
                        b.minZ = b.startPos.z - 1.0f;
                        b.maxZ = b.startPos.z + 3.0f;
                    }

                    //check if direction needs to be randomly changed
                    if (changeDirection == 1)
                    {
                        v.direction.x *= randomDirection.x;
                        v.direction.z *= randomDirection.z;
                    }

                    //set new XZ positions
                    t.Value.x += (v.speed.x * v.direction.x);
                    t.Value.z += (v.speed.z * v.direction.z);

                    //checks if nextPos.x is out of bounds. If so, x direction is reversed
                    if (t.Value.x <= b.minX || t.Value.x >= b.maxX) { v.direction.x *= -1.0f; }
                    //checks if nextPos.z is out of bounds. If so, z direction is reversed
                    if (t.Value.z <= b.minZ || t.Value.z >= b.maxZ) { v.direction.z *= -1.0f; }

                    //NOTE: THIS WILL NOT WORK!!! Can't find a way to reference just ONE entity from another!!!!!
                    //check if collision with partner cube has occurred
                    //float radius = Math.Sqrt( Math.Abs(t.Value.x - EntityManager. e.Translation.x ) + () );
                    //Entity partner = e.partner;
                    //var pTranslation = EntityManager.GetComponentData<Translation>(partner);
                    //var pt = GetComponent<Translation>(e.partner);
                    //var pt = GetComponentDataFromEntity<Translation>(partner);

                    //re-set new XZ positions in case there was a change in direction
                    t.Value.x += (v.speed.x * v.direction.x);
                    t.Value.z += (v.speed.z * v.direction.z);

                    //added this for increased computation time per knight
                    //float value = 0f;
                    //for (int i = 0; i < 50000; ++i)
                    //{
                    //    value = math.exp10(math.sqrt(value));
                    //}
                }
            });
        
        
        
        }
        //*/


    }
}
