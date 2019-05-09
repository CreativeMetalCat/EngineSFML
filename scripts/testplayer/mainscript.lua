
function SpawnPlayer (actor, sprite, size, collisionShape, location, playerControlled,worldContext,path)

  CTestPlayer.Duplicate( sprite, size, collisionShape, location, playerControlled,worldContext,path);
end

function FireProjectile(this,sprite,worldContext)
  --Create Impluse and Location Using Vector2
  local spawnImpulse = Vector2();
  spawnImpulse.x=200;
  spawnImpulse.y=0;


  local size=Vector2();
  size.x=32;
  size.y=32;

  --due to the fact that projectiles use rectangle shaped collision we can easily find center
  local impulseLocalLocation=Vector2();
  impulseLocalLocation.y = size.y/2;
  impulseLocalLocation.x = size.x/2;



  local spawnLocation=Vector2();
  spawnLocation.x = this.Location.x + 32+size.x/2;
  spawnLocation.y = this.Location.y;

  TestProjectile.Create(spawnImpulse,impulseLocalLocation,sprite,size,spawnLocation,worldContext,this.Path)

end

function OnBeginCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions
end

function OnEndCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions

end
