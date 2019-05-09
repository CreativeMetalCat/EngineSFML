--template file
--Main weapon data table that holds all possible data that can be used



Weapon=
{
  AmmoPerClip = 10000,
  AmmoLeftInTheClip = 10000,
  TimeBetweenShots = 0.1,
  TimeSinceLastShot = 0,
  Shot = false
}

--LeftInTheClip=10;

function CanShoot ()
  if(Weapon.TimeSinceLastShot==0.0) and (Weapon.AmmoLeftInTheClip>0) then
    return true;
  else
    return false;
  end
end

function Update (this,deltaSeconds)
  if Weapon.Shot==true then
    Weapon.TimeSinceLastShot=Weapon.TimeSinceLastShot+deltaSeconds;
    if (Weapon.TimeSinceLastShot>Weapon.TimeBetweenShots) then
      Weapon.TimeSinceLastShot=0.0;
      Weapon.Shot=false;
    end

  end

end

function Shoot(weapon,sprite,Location,angle,worldContext)
if CanShoot()==true then

    local spawnImpulse = Vector2();
    spawnImpulse.x=100*math.cos(angle);
    spawnImpulse.y=100*math.sin(angle);


    local size=Vector2();
    size.x=32;
    size.y=32;

    --due to the fact that projectiles use rectangle shaped collision we can easily find center
    local impulseLocalLocation=Vector2();
    impulseLocalLocation.y = size.y/2;
    impulseLocalLocation.x = size.x/2;



    local spawnLocation=Vector2();
    spawnLocation.x = Location.x;
    spawnLocation.y = Location.y;

    TestProjectile.Create(spawnImpulse,impulseLocalLocation,sprite,size,spawnLocation,worldContext,weapon.Path)
    Weapon.AmmoLeftInTheClip=Weapon.AmmoLeftInTheClip-1;

    Weapon.Shot=true;
  else
    --if can not shoot
  end
end
