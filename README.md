# VAWT Simulation
## Cleanup
Remove all the old generated mesh files

```rm -r constant/polyMesh```

```rm -r processor*```

```rm -r 2```

## Run
If it has not been done before, generate the feature edge mesh

```surfaceFeatureExtract```

Generate the background block mesh

```blockMesh```

Decompose the case for parallel processing

```decomposePar -constant -noZero```

Run the case in parallel on 24 threads

```mpirun --use-hwthread-cpus -np 24 snappyHexMesh -overwrite -parallel```

Reconstruct the mesh

```reconstructParMesh -mergeTol 1e-06 -constant```

Renumber the mesh

```renumberMesh -overwrite -constant```

Use the createPatch utility to change the patch types to AMI to allow information transfer along the rotating mesh boundary

```createPatch -overwrite```

Run mesh quality check

```checkMesh -constant```

Decompose Mesh again for simulation

