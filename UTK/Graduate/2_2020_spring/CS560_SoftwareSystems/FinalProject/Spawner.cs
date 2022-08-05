using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Entities;
using Unity.Jobs;
using UnityEngine.Jobs;
using Unity.Burst;

//global game state manager reference
using gsm = GameStateManager;

/*
//add new components to GameObjects.
public class eTimeGO { public float Value; } //NOTE: This is probably not necessary!!!

public class BoundaryGO
{
    public float minX;
    public float maxX;
    public float minZ;
    public float maxZ;
    public Vector3 startPos;
}
public class VelocityGO { public Vector3 speed; public Vector3 direction; }
public class eEntityGO { public float id; }//public Entity partner; }//NOTE: Don't know if sticking an Entity in another Entity will work. It should, buuuuutt....
//*/

public class Spawner: MonoBehaviour //NOTE: I THINK I want to inherit from "some Entity class"
{
    // creates new "shell" of groups
    //[BurstCompile]
    public static void SpawnShell()
    {
        //intially update shell number and number of knights
        gsm.shellNum += 1;

        //spawn groups in x direction --- z coordinate remains fixed
        for (int x = 0; x <= gsm.shellNum; ++x)
        {
            SpawnGroup( (int)(x * gsm.groupSizeX * gsm.dsX) , (int)((gsm.shellNum) * gsm.groupSizeZ * gsm.dsZ) );
        }

        //spawn groups in z direction --- x coordinate remains fixed
        for (int z = 0; z <= gsm.shellNum - 1; ++z)
        {
            SpawnGroup((int)(gsm.shellNum * gsm.groupSizeX * gsm.dsX), (int)(z * gsm.groupSizeZ * gsm.dsZ));
        }

    }
        
    // creates new group of game models
    //does not use shellNum
    //[BurstCompile]
    public static void SpawnGroup(int startX, int startZ)
    {
        //Debug.Log("In Spawner. GameStateManager.ECS was FALSE!");
        for (int z = 0; z < gsm.groupSizeZ; ++z)
        {
            for (int x = 0; x < gsm.groupSizeX; ++x)
            {
                if (!(x == 0 && z == 0 && startX == 0 && startZ == 0)) 
                {
                    Instantiate(gsm.desertSquare, new Vector3(startX + x * gsm.dsX, 0, startZ + z * gsm.dsZ), new Quaternion(0, 0, 0, 0));
                    if (gsm.JOBS)
                    {
                        gsm.knightTransforms.Add(Instantiate(gsm.knight, new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1), new Quaternion(0, 0, 0, 0)).transform);
                        gsm.knightTransforms.Add(Instantiate(gsm.knight1, new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1), new Quaternion(0, 180, 0, 0)).transform);

                        gsm.knightDirectionList.Add(new Vector3(1.0f, 0.0f, 1.0f));
                        gsm.knightDirectionList.Add(new Vector3(-1.0f, 0.0f, 1.0f));

                        gsm.knightStartPosList.Add(new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1));
                        gsm.knightStartPosList.Add(new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1));

                        gsm.knightIDList.Add(0.0f);
                        gsm.knightIDList.Add(1.0f);
                    }
                    else
                    {
                        GameObject tempK0 = Instantiate(gsm.knight, new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1), new Quaternion(0, 0, 0, 0));
                        GameObject tempK1 = Instantiate(gsm.knight1, new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1), new Quaternion(0, 180, 0, 0));

                        //NOTE: had to remove "static" keyword from variables in MyRotator.cs so I could reference them here!!!
                        tempK0.GetComponent<MyRotator>().startPos = new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) - 1);
                        tempK1.GetComponent<MyRotator>().startPos = new Vector3(startX + x * gsm.dsX, 0, (startZ + z * gsm.dsZ) + 1);

                        tempK0.GetComponent<MyRotator>().speed = new Vector3(0.01f, 0.0f, 0.01f);
                        tempK1.GetComponent<MyRotator>().speed = new Vector3(0.01f, 0.0f, 0.01f);

                        tempK0.GetComponent<MyRotator>().direction = new Vector3(1.0f, 0.0f, 1.0f);
                        tempK1.GetComponent<MyRotator>().direction = new Vector3(-1.0f, 0.0f, 1.0f);

                        tempK0.GetComponent<MyRotator>().ID = 0.0f;
                        tempK1.GetComponent<MyRotator>().ID = 1.0f;


                    }

                    gsm.numOfKnights += 2;
                }
                else { /*Do Nothing*/ }      
            }
        }
    }
}

// CODE STORAGE-----------------------------------------------------------------------------------------------------------------------------
/*
  // destroys group of game models previously created with SpawnGroup
  //[BurstCompile]
  public static void UnspawnGroup()
  {
      if (gsm.ECS)
      {
          Debug.Log("In Spawner. GameStateManager.ECS was TRUE!");
      }
      else
      {
          Debug.Log("In Spawner. GameStateManager.ECS was FALSE!");
      }
  }
*/
