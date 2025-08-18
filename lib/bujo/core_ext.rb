# frozen_string_literal: true
# typed: strict

class Array
  extend T::Sig

  sig { returns(T.self_type) }
  def deep_symbolize_keys!
    each_with_index do |value, index|
      value = T.unsafe(value)
      self[index] = (value.is_a?(Hash) || value.is_a?(Array)) ? value.deep_symbolize_keys! : value
    end
    self
  end
end

class Hash
  extend T::Sig

  sig { returns(T.self_type) }
  def deep_symbolize_keys!
    keys.each do |key|
      key = T.unsafe(key)
      value = delete(key)
      value = T.unsafe(value)
      self[key.to_sym] = (value.is_a?(Hash) || value.is_a?(Array)) ? value.deep_symbolize_keys! : value
    end
    self
  end
end
