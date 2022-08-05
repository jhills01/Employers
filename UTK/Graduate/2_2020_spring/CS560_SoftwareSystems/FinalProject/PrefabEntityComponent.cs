//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
using System.Numerics;
using Unity.Entities;

//using gsm = GameStateManager;

[GenerateAuthoringComponent]
public struct PrefabEntityComponent : IComponentData
{
    public Entity knightEntity;
    public Entity knight1Entity;
    public Entity desertSquare;
}
