--this file defines basic movement,collision ets. for all character based objects

--value should be used only for the direction
--e.g. -1 or 1
function Move(actor,value)

  local d = actor:GetLinearVelocity();

  local vel = d.x;


  local res =tonumber(actor.MaxVelocity.x)*value;

  local velChange=res-vel;


  local impulse = b2Vector();

  impulse.x=velChange*64;
  impulse.y=0;


  actor:ApplyLinearImpulse(impulse.x,impulse.y);
end


--value should be used only for the direction
--e.g. -1 or 1
function MoveY(actor,value)

  local d = actor:GetLinearVelocity();

  local vel = d.y;


  local res =tonumber(actor.MaxVelocity.y)*value;

  local velChange=res-vel;


  local impulse = b2Vector();

  impulse.y=velChange*64;
  impulse.x=0;


  actor:ApplyLinearImpulse(impulse);
end

function OnBeginCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions
end

function OnEndCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions
  
end
