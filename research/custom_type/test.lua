function arrayTest()
   a = array.new(1000)
   print(a)
   print(array.size(a))
   for i=1,1000 do
      array.set(a, i, 1/i)
   end
   print(array.get(a, 10))
end

function treeTest()
   print("creating new tree root")
   local tn = treenode.new("root", 0)
   print(tn)
   print("adding 3 child nodes to our root")
   buildTree(tn)

   for n in tn:children() do
      print(n)
   end

   print("add two new nodes to the second child")
   appendNode(tn, "node2", treenode.new("subnode1", 1.1))
   appendNode(tn, "node2", treenode.new("subnode2", "2.2"))
   collectgarbage(collect)
   print("garbage collected. this will make sure that we correctly added children to the tree")

   print("remove second child while iterating over children iterator.")
   for n in tn:children() do
      if n:getname() == "node2" then
         print("removing node with name 'node2'")
         n:rm()
      end
   end

   print("root's children that left in tree")
   for n in tn:children() do
      print(n)
   end

   print("change root's value")
   tn:set("another value")
   print(tn)
   print("change root's name")
   tn:setname("another root's name")
   print(tn)

   print("tests finished")
end

function buildTree(tr)
   local tn1 = treenode.new("node1", 1)
   local tn2 = treenode.new("node2", 2)
   local tn3 = treenode.new("node3", 3)
   tr:add(tn1)
   tr:add(tn2)
   tr:add(tn3)
end

function appendNode(treeRoot, nodeName, newNode)
   for n in treeRoot:children() do
      if n:getname() == nodeName then
         n:add(newNode)
         return
      end
   end
end
