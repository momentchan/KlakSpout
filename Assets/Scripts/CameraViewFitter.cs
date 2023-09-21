using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class CameraViewFitter : MonoBehaviour
{
    [SerializeField] private Camera cam;
    void Update()
    {
        var size = cam.orthographicSize * 2;
        transform.localScale = new Vector3(size * cam.aspect, size , 1);
    }
}
