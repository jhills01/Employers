using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Entities;
using Unity.Transforms;
using Unity.Mathematics;
using Unity.Jobs;

using gsm = GameStateManager;
using km = KnightManager;
using Unity.Collections;

//add new components to Entities.
public struct eTime : IComponentData { public float Value; } //NOTE: This is probably not necessary!!!

public struct Boundary : IComponentData 
{ 
    public float minX;
    public float maxX;
    public float minZ;
    public float maxZ;
    public Vector3 startPos;
}
public struct Velocity : IComponentData { public Vector3 speed; public Vector3 direction; }
public struct eEntity : IComponentData { public float id; }//public Entity partner; }//NOTE: Don't know if sticking an Entity in another Entity will work. It should, buuuuutt....


public class EntitySpawnerSystem : ComponentSystem
{
    //Entity parameters
    private readonly Vector3 uniSpeed = new Vector3(0.01f, 0.0f, 0.01f);

    public readonly float[] directionChoices = { 1.0f, -1.0f };
    public static float timeInterval = 600f;
    public static float time = timeInterval;

public int counter = -1;

    protected override void OnUpdate()
    {
        //if (Input.GetKeyDown(KeyCode.UpArrow))
        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (gsm.ECS) //NOTE: this prevent me from accidently running unwanted code by hitting space during another version
            {
                //NOTE: Debug.Log does add some overhead, but I'm still getting FPS stutter when pressing UpArrow
                SpawnShell();
                //Debug.Log("gsm.knights.Count: " + gsm.knightTransforms.Count);
                Debug.Log("gsm.numOfKnights:  " + gsm.numOfKnights);
            }
        }

///*
        //NOTE: This Entities.ForEach is here b/c I need to a class to inherit from ComponentSystem instead of SystemBase like in EntityRotationJobSystem
        if (!gsm.JOBS && gsm.ECS)
        {
            int changeDirection = 0; //NOTE: Don't need to assign to this in ForEach b/c it is initalized every time!! what a waste!
            Vector3 randomDirection = new Vector3(1.0f, 0.0f, -1.0f);
            ComponentDataFromEntity<Velocity> VelocityFromEntity = GetComponentDataFromEntity<Velocity>(true); //Needed to reference component data from Entity currently seen in ForEach below

            //decrease time and check if ZERO is reached
            --time;
            if (time <= 0.0f)
            {
                time = timeInterval;

                randomDirection.x = directionChoices[gsm.random.NextInt(directionChoices.Length)];
                randomDirection.z = directionChoices[gsm.random.NextInt(directionChoices.Length)];

                changeDirection = 1;
            }

            Entities.ForEach((Entity ent, ref Rotation r, ref Translation t) =>
            //Entities.ForEach((Entity e, ref Rotation r, ref Translation t, ref Velocity v, ref Boundary b) =>
            //Entities.ForEach((Entity ent, ref Rotation r, ref Translation t, ref Velocity v, ref Boundary b, in eEntity e, in NonUniformScale nus) =>
            //Entities.ForEach((ref Rotation rotation) =>
            {
                //if ( (EntityManager.GetName(ent).ToLower()).Contains("knight") )
                if ( (EntityManager.GetName(ent).ToLower()).Contains("knight") )
                {
                    //NOTE: This might be making a copy of Velocity which would mean I'm not changing values within component
                    Velocity v = VelocityFromEntity[ent]; //WARNING: This is a COPY of the Entity 
                    Boundary b = EntityManager.GetComponentData<Boundary>(ent);
                    eEntity e = EntityManager.GetComponentData<eEntity>(ent);

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
                    //NOTE: I can't pass Velocity component directly in ForEach. See above for the really odd way to reference component data from Entity currently seen in ForEach 
                    if (changeDirection == 1)
                    {
                        v.direction.x *= randomDirection.x;
                        v.direction.z *= randomDirection.z;
                    }

                    //set new XZ positions
                    t.Value.x += (v.speed.x * v.direction.x);
                    t.Value.z += (v.speed.z * v.direction.z);

                    //checks if nextPos.x is out of bounds. If so, x direction is reversed
                    if (t.Value.x <= b.minX || t.Value.x >= b.maxX) 
                    {
                        v.direction.x *= -1.0f; 
                    }
                    //checks if nextPos.z is out of bounds. If so, z direction is reversed
                    if (t.Value.z <= b.minZ || t.Value.z >= b.maxZ) 
                    { 
                        v.direction.z *= -1.0f;
                    }

                    //re-set new XZ positions in case there was a change in direction
                    t.Value.x += (v.speed.x * v.direction.x);
                    t.Value.z += (v.speed.z * v.direction.z);

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
            });
        }
//*/

    }

    //protected override SpawnShell()
    public void SpawnShell()
    {
        Debug.Log("Entering EntitySpawnerSystem");

        //intially update shell number and number of knights
        gsm.shellNum += 1;

        //spawn groups in x direction --- z coordinate remains fixed
        for (int x = 0; x <= gsm.shellNum; ++x)
        {
            SpawnGroup((int)(x * gsm.groupSizeX * gsm.dsX), (int)((gsm.shellNum) * gsm.groupSizeZ * gsm.dsZ));
        }

        //spawn groups in z direction --- x coordinate remains fixed
        for (int z = 0; z <= gsm.shellNum - 1; ++z)
        {
            SpawnGroup((int)(gsm.shellNum * gsm.groupSizeX * gsm.dsX), (int)(z * gsm.groupSizeZ * gsm.dsZ));
        }

    }

    //protected override SpawnGroup(int startX, int startZ)
    public void SpawnGroup(int startX, int startZ)
    {
        for (int z = 0; z < gsm.groupSizeZ; ++z)
        {
            for (int x = 0; x < gsm.groupSizeX; ++x)
            {
                Entities.ForEach((ref PrefabEntityComponent prefabEntityComponent) => 
                {
                    //pull all 3 entities from PrefabEntityComponents
                    Entity knightEntity = EntityManager.Instantiate(prefabEntityComponent.knightEntity);
                    Entity knight1Entity = EntityManager.Instantiate(prefabEntityComponent.knight1Entity);
                    Entity desertSquare = EntityManager.Instantiate(prefabEntityComponent.desertSquare);

                    //NOTE: The EntityManager.Instantiate() for the REAL knights is crashing Unity!!!!!!!!!

                    //set knight transforms----------------------------------------------------------------------------------------------------------------
                    EntityManager.SetComponentData(knightEntity,
                        new Translation 
                        { 
                            Value = new float3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1),
                        });
                    
                    EntityManager.SetComponentData(knightEntity,
                        new Rotation
                        {
                            Value = new Quaternion(0, 0, 0, 1)
                        });

                    EntityManager.AddComponentData(knightEntity,
                        new Velocity
                        {
                            direction = new Vector3(1.0f, 0.0f, 1.0f),
                            speed = uniSpeed
                        });

                    EntityManager.AddComponentData(knightEntity,
                        new eTime
                        {
                            Value = 0.0f
                        });

                    EntityManager.AddComponentData(knightEntity,
                        new Boundary
                        {
                            minX = 0.0f,
                            maxX = 0.0f,
                            minZ = 0.0f,
                            maxZ = 0.0f,
                            startPos = new float3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1),
                        });

                    EntityManager.AddComponentData(knightEntity,
                        new eEntity
                        {
                            id = 0.0f//, partner = knight1Entity
                        });

                    //set knight1 transforms------------------------------------------------------------------------------------------------------
                    EntityManager.SetComponentData(knight1Entity,
                       new Translation
                       {
                           Value = new float3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1)
                       });
                   EntityManager.SetComponentData(knight1Entity,
                       new Rotation
                       {
                           Value = new Quaternion(0, 0, 0, 1)
                       });
                    
                    EntityManager.AddComponentData(knight1Entity,
                        new Velocity
                        {
                            direction = new Vector3(-1.0f, 0.0f, 1.0f),
                            speed = uniSpeed
                        });

                    EntityManager.AddComponentData(knight1Entity,
                        new eTime
                        {
                            Value = 0.0f
                        });

                    EntityManager.AddComponentData(knight1Entity,
                        new Boundary
                        {
                            minX = 0.0f,
                            maxX = 0.0f,
                            minZ = 0.0f,
                            maxZ = 0.0f,
                            startPos = new float3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1),
                        });

                    EntityManager.AddComponentData(knight1Entity,
                        new eEntity
                        {
                            id = 1.0f//,
                            //partner = knightEntity
                        });


                    //desertSquare--------------------------------------------------------------------------------------------------------------
                    EntityManager.SetComponentData(desertSquare,
                        new Translation
                        {
                            Value = new float3(startX + x * gsm.dsX, 0, startZ + z * gsm.dsZ)
                        });
                    EntityManager.SetComponentData(desertSquare,
                        new Rotation
                        {
                            Value = new Quaternion(0, 0, 0, 0)
                        });

                    //EntityManager.AddComponentData(desertSquare,
                    //    new Velocity
                    //    {
                    //        direction = new Vector3(1.0f, 0.0f, 1.0f),
                    //        speed = uniSpeed
                    //    });

                    //EntityManager.AddComponentData(desertSquare,
                    //    new eTime
                    //    {
                    //        Value = 0.0f
                    //    });
                    
                    //EntityManager.AddComponentData(desertSquare,
                    //    new Boundary
                    //    {
                    //        minX = 0.0f,
                    //        maxX = 0.0f,
                    //        minZ = 0.0f,
                    //        maxZ = 0.0f,
                    //        startPos = new float3(startX + x * gsm.dsX, 0, startZ + z * gsm.dsZ)
                    //    });

                    //EntityManager.AddComponentData(desertSquare,
                    //    new eEntity
                    //    {
                    //        id = 2.0f//,
                    //        //partner = knightEntity
                    //    });
                });

                gsm.numOfKnights += 2;
            }
        }
    }
}