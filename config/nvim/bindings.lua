vim.keymap.set("n", "<leader>ns", function()
  -- TODO: this is going to have to call `bujo list` and pass it to a picker
  -- because `bujo search` can't run interactively
end, { desc = "Bujo: search documents" })

vim.keymap.set("n", "<leader>nn", function()
  bujo_edit("bujo spread current")
end, { desc = "Bujo: current spread" })

vim.keymap.set("n", "<leader>nf", function()
  bujo_edit("bujo spread next")
end, { desc = "Bujo: spread next" })

vim.keymap.set("n", "<leader>nF", function()
  local current_spread = vim.fn.expand("%:p")
  bujo_edit("bujo spread next " .. current_spread)
end, { desc = "Bujo: spread forward" })

vim.keymap.set("n", "<leader>nb", function()
  bujo_edit("bujo spread previous")
end, { desc = "Bujo: spread previous" })

vim.keymap.set("n", "<leader>nB", function()
  local current_spread = vim.fn.expand("%:p")
  bujo_edit("bujo spread previous " .. current_spread)
end, { desc = "Bujo: spread backward" })

vim.keymap.set("n", "<leader>nS", function()
  vim.fn.system("bujo sync")
end, { desc = "Bujo: sync" })
