#!/usr/local/bin/ruby
##

#part='SET_%{obj}(universe, %{objNbr}, %{posX}, %{posY}, %{mass}, %{velX}, %{velY});'
part='      {"nbr": %{nbr}, "x": %{posX}, "y": %{posY}, "z": %{posZ}, "vx": %{velX}, "vy":%{velY}, "vz":%{velZ}, "mass": %{mass}},'

radius=1000
universe_radius=10000
nbr_part=ARGV[0].to_i;
density_neg=8
def function (radius, x)
  return Math.sqrt(radius * radius - x * x)
end

def get_co (inout, radius, universe_radius)
  begin
    x=rand(universe_radius) * (rand(2) < 1 ? 1 : -1)
    y=rand(universe_radius) * (rand(2) < 1 ? 1 : -1)
  end until inout < 1 ? Math.hypot(x, y) < radius : Math.hypot(x, y) > radius

  return x, y
end

File.open("#{ARGV[0].to_i}.json", 'w') do |fo|
  $stdout = fo
  puts '{
        "Too Close": 0.1,
        "Save Dir": "tmp\/",
        "Save File": "file.json",
        "Universe Radius": 1000,
   "Num Tic": 1,
   "Too Close": 1,
   "Too Far": 1000,
   "Object Size": 1000,
   "Precision": 10,
   "Initial Space Objects": ['
i=0
(0..nbr_part).each do |n|
  puts part % {:nbr => n, :posX => (i += 1), :posY => (i += 1), :posZ => (i +=1), :velX => (i += 1), :velY => (i += 1), :velZ => (i += 1), :mass=> 2}
#  puts part % {:nbr => n, :posX => i, :posY => i, :posZ => i, :velX => 2, :velY => 2, :velZ => 2, :mass=> 2}
#  i += 1
end

#(nbr_part.. density_neg * nbr_part).each do |n|
#  x, y=get_co(1, radius, universe_radius)
#  puts part % {:obj => "OBJ", :objNbr => n, :posX =>x, :posY => y, :mass=>-2, :velX => x - 1, :velY => y - 1}
#  puts part % {:obj => "TMP", :objNbr => n, :posX =>x, :posY => y, :mass=>-2, :velX => x - 1, :velY => y - 1}
#end
puts '],
}'
end
