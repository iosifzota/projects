declare Flatten in
fun {Flatten Xs}
   proc {FlattenD Xs ?Ds}
      case Xs
      of nil then Y in Ds=Y#Y
      [] X|Xr andthen {IsList X} then Y1 Y2 Y3 in
         Ds = Y1#Y3
         {FlattenD X Y1#Y2}
         {FlattenD Xr Y2#Y3}
      [] X|Xr then Y1 Y2 in
         Ds = (X|Y1)#Y2
         {FlattenD Xr Y1#Y2}
      end
   end
   Ys
   in 
   {FlattenD Xs Ys#nil}
   Ys
end
