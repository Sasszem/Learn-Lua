local case = { }

function case.String(cfg, st, node)
  local firstline, _ = getfirstline(node,true)
  local lastline = getlastline(node)
  for line=firstline+1, lastline do
    st.indentation[line]=false
  end
end

function case.Table(cfg, st, node)

  if not cfg.indenttable then
    return
  end

  -- Format only inner values across several lines
  local firstline, firstindex = getfirstline(node,true)
  local lastline = getlastline(node)
  if #node > 0 and firstline < lastline then

    -- Determine first line to format
    local firstnode = unpack(node)
    local childfirstline, childfirstindex = getfirstline(firstnode)

    -- Determine last line to format
    local lastnode = #node == 1 and firstnode or node[ #node ]
    local childlastline = getlastline(lastnode)

    -- Actual formating
    indent(cfg, st, childfirstline, childfirstindex, childlastline, node)
  end
end

--------------------------------------------------------------------------------
-- Statements formatters
--------------------------------------------------------------------------------
function case.Call(cfg, st, node)
  local expr, firstparam = unpack(node)
  if firstparam then
    indentparams(cfg, st, firstparam, node[#node], node)
  end
end

function case.Do(cfg, st, node, parent)
  -- Ignore empty node
  if #node == 0 or not parent then
    return
  end
  indentchunk(cfg, st, node, parent)
end
