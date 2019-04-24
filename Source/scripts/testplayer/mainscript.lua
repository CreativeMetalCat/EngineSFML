
function SpawnPlayer (actor, sprite, size, collisionShape, location, playerControlled,worldContext,path)
  CTestPlayer.Duplicate( sprite, size, collisionShape, location, playerControlled,worldContext,path);
end

function OnBeginCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions
end

function OnEndCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions

end
