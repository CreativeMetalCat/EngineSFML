function OnBeginCollision (actor,arb,otherActor,worldContext)
  --Copy this function to your script and add actions
  if(otherActor:GetClassID()==4) then
    print(actor);
  end
end

function OnEndCollision (actor,arb,otherActor)
  --Copy this function to your script and add actions
end
