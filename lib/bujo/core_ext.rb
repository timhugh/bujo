# frozen_string_literal: true
# typed: true

class Array
  def deep_symbolize_keys!
    each_with_index do |value, index|
      value = T.cast(value, T.untyped)
      self[index] = (value.is_a?(Hash) || value.is_a?(Array)) ? value.deep_symbolize_keys! : value
    end
  end
end

class Hash
  def deep_symbolize_keys!
    keys.each do |key|
      key = T.cast(key, T.untyped)
      value = delete(key)
      value = T.cast(value, T.untyped)
      self[key.to_sym] = (value.is_a?(Hash) || value.is_a?(Array)) ? value.deep_symbolize_keys! : value
    end
    self
  end
end
