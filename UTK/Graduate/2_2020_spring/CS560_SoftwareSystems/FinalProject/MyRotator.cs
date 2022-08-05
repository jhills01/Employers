using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Mathematics;

using gsm = GameStateManager;

public class MyRotator : MonoBehaviour 
{
    //ORIGINAL CODE
    //public float speed;
    //public Vector3 direction = Vector3.zero;// Use this for initialization

    /*
        void Start () 
        {
            //ORIGINAL CODE
            //speed = 20;
            //direction = new Vector3(0,1,0);
        }
    */

    public Vector3 startPos, direction, speed;
    public float minX, maxX, minZ, maxZ;
    public float ID;

	// Update is called once per frame
	void Update()
	//void FixedUpdate()
	{
		if (!gsm.JOBS)
		{
			transform.rotation = Quaternion.Euler(0, 0.2f, 0) * transform.rotation;
            //transform.eulerAngles = new Vector3(0f, ClampAngle(transform.eulerAngles.y), 0f);

            minX = startPos.x - 2.0f;
            maxX = startPos.x + 2.0f;

            if (ID == 0f)
            {
                minZ = startPos.z - 1.0f;
                maxZ = startPos.z + 3.0f;
            }
            else
            {
                minZ = startPos.z - 3.0f;
                maxZ = startPos.z + 1.0f;
            }

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
            }
            //checks if nextPos.z is out of bounds. If so, z direction is reversed
            if (transform.position.z <= minZ || transform.position.z >= maxZ)
            {
                direction.z *= -1.0f;
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
		//ORIGINAL CODE
		//transform.Rotate(direction * Time.deltaTime * speed, Space.World);
	}

}
