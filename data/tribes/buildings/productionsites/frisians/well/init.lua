dirname = path.dirname (__file__)

tribes:new_productionsite_type {
   msgctxt = "frisians_building",
   name = "frisians_well",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("frisians_building", "Well"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "small",

   buildcost = {
      log = 1,
      granite = 2,
      brick = 1
   },
   return_on_dismantle = {
      granite = 1,
      brick = 1
   },

   spritesheets = {
      working = {
         directory = dirname,
         basename = "working",
         hotspot = {19, 33},
         frames = 10,
         columns = 5,
         rows = 2,
         fps = 10
      }
   },
   animations = {
      idle = {
         directory = dirname,
         basename = "idle",
         hotspot = {19, 33}
      }
   },

   aihints = {
      collects_ware_from_map = "water",
      basic_amount = 1
   },

   working_positions = {
      frisians_carrier = 1
   },

   outputs = {
      "water"
   },

   indicate_workarea_overlaps = {
      frisians_well = false,
   },

   programs = {
      work = {
         -- TRANSLATORS: Completed/Skipped/Did not start working because ...
         descname = _"working",
         actions = {
            "sleep=20000",
            "animate=working 20000",
            "mine=water 1 100 65 2",
            "produce=water",
         }
      },
   },

   out_of_resource_notification = {
      -- Translators: Short for "Out of ..." for a resource
      title = _"No Water",
      heading = _"Out of Water",
      message = pgettext ("frisians_building", "The carrier working at this well can’t find any water in his well."),
      productivity_threshold = 33
   },
}
