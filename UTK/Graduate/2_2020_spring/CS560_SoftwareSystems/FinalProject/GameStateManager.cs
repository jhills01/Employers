using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Entities;
using Unity.Transforms;
using Unity.Collections;

public class GameStateManager : MonoBehaviour//, IConvertGameObjectToEntity
{
    [SerializeField] public static bool ECS; //"SerializeField" doesn't seem to work here for some reason
    [SerializeField] public static bool JOBS; //"SerializeField" doesn't seem to work here for some reason
    public static int killme;
    public static string key;
    public static int shellNum;
    public static int groupSizeX;
    public static int groupSizeZ;
    public static int numOfKnights;
    public static GameObject desertSquare;
    public static GameObject knight;
    public static GameObject knight1;
    public static GameObject updateManager;
    //public static Entity entityManager;
    public static List<GameObject> knightGOs; //NOTE: not sure I even need this
    public static List<Transform> knightTransforms;
    
    //public static List<Vector3> knightSpeedList;
    public static List<Vector3> knightDirectionList;
    public static List<Vector3> knightStartPosList;
    public static List<float> knightIDList;
    //public static Vector3[] knightSpeedArr;
    //public static Vector3[] knightDirectionArr;

    public static readonly float[] directionChoices = { 1.0f, -1.0f };
    public static float timeInterval = 600f;
    public static float time = timeInterval;
    public static int changeDirection = 0; 
    public static Vector3 randomDirection = new Vector3(1.0f, 0.0f, -1.0f);


    //"ds" == desertSquare
    public static float dsX;
    //public static float dsY;
    public static float dsZ;

    public static float deltaTime;
    public static Unity.Mathematics.Random random = new Unity.Mathematics.Random(0x6E624EB7u); //Found this hex value @ https://forum.unity.com/threads/runtime-error-on-random.548449/

    //constructor --- allows assignment to readonly variables
    //static GameStateManager()
    //{

    //    dsX = desertSquare.GetComponent<MeshRenderer>().bounds.size.x;
    //    //dsY = desertSquare.GetComponent<MeshRenderer>().bounds.size.y;
    //    dsZ = desertSquare.GetComponent<MeshRenderer>().bounds.size.z;
    //}

    // Start is called before the first frame update
    void Start()
    {
        //Initialize all game data
        
        //---NOTE: "ECS = true && JOBS = false" does not work as intended b/c there's no way to update DIRECTION of Entities. 
        //          ...Velocity can't be passed as ref and there doesn't seem to be a way to reference Velocity directly for currently selected Entity. Just stuck!
        ECS = false;
        //ECS = true;
        //JOBS = false; //if FALSE and KILLME, ECS = FALSE too!!!
        JOBS = true;
        //killme = 0;
        killme = 1; //NOTE: Uncomment loop for "ECS = true && JOBS = true"

        key = null;
        shellNum = -1;
        groupSizeX = 1; //Change to 1 when NOT using JOBS
        groupSizeZ = 1; //Change to 1 when NOT using JOBS
        numOfKnights = 2;

        desertSquare = GameObject.Find("DesertSquare");
        knight = GameObject.Find("knightprefab");
        knight1 = GameObject.Find("knightprefab(1)");
        updateManager = GameObject.Find("UpdateManager");

        dsX = desertSquare.GetComponent<MeshRenderer>().bounds.size.x;
        //dsY = desertSquare.GetComponent<MeshRenderer>().bounds.size.y;
        dsZ = desertSquare.GetComponent<MeshRenderer>().bounds.size.z;

Debug.Log("dsX: " + dsX);        
Debug.Log("dsZ: " + dsZ);        

        //random translation variables


        //disable all traditional gameObjects except GameStateManager
        if (ECS)
        {
            Debug.Log("Disabling initial gameObjects");
            knight.SetActive(false);
            knight1.SetActive(false);
            updateManager.SetActive(false);
            desertSquare.SetActive(false);
            numOfKnights = 0;
        }

        //NOTE: NO ENTITIES exist at this point, so I can't store transforms. Entities are created in EntitySpawnerSystem
        if (JOBS && !ECS)
        {
            knightIDList = new List<float>();
            //knightIDList.Add(0.0f);
            //knightIDList.Add(1.0f);
            knightIDList.Add(1.0f);
            knightIDList.Add(0.0f);

            knightStartPosList = new List<Vector3>();
            knightStartPosList.Add(new Vector3(0.0f, 0.0f, 1.0f));
            knightStartPosList.Add(new Vector3(0.0f, 0.0f, -1.0f));

            knightDirectionList = new List<Vector3>();
            knightDirectionList.Add(new Vector3(1.0f, 0.0f, 1.0f));
            knightDirectionList.Add(new Vector3(-1.0f, 0.0f, 1.0f));

            knightTransforms = new List<Transform>();
            knightTransforms.Add(knight.transform);
            knightTransforms.Add(knight1.transform);
            
            KnightManager.initializeKnights();
        }

        if (!ECS && !JOBS)
        {
            //knight.GetComponent<MyRotator>().ID = 0.0f;
            //knight1.GetComponent<MyRotator>().ID = 1.0f;
            knight.GetComponent<MyRotator>().ID = 1.0f;
            knight1.GetComponent<MyRotator>().ID = 0.0f;

            knight.GetComponent<MyRotator>().startPos = new Vector3(0.0f, 0.0f, 1.0f);
            knight1.GetComponent<MyRotator>().startPos = new Vector3(0.0f, 0.0f, -1.0f);

            knight.GetComponent<MyRotator>().speed = new Vector3(0.01f, 0.0f, 0.01f);
            knight1.GetComponent<MyRotator>().speed = new Vector3(0.01f, 0.0f, 0.01f);

            knight.GetComponent<MyRotator>().direction = new Vector3(1.0f, 0.0f, 1.0f);
            knight1.GetComponent<MyRotator>().direction = new Vector3(-1.0f, 0.0f, 1.0f);
        }

    }

/*
    // Update is called once per frame
    void Update()
    {
        //NOTE: I should make Update() of all scripts waiting for input to be coroutines        
    }
 */
}
