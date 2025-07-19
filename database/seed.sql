-- Sample data for initial setup

-- Admin user with password "adminpass"
INSERT INTO users(username, password_hash, password_salt, role) VALUES
  ('admin', '0ba52449484358aec21151459f761b9122028c2a77fbbe41d86ecd59d2b3936e', 'eea52bddd5194422a84159ea8e401f7f', 'admin');

-- Example products
INSERT INTO products(name, price, discount) VALUES
  ('Coffee', 5.00, 0),
  ('Tea', 3.50, 0);

-- Inventory entries
INSERT INTO inventory(product_id, quantity) VALUES
  (1, 50),
  (2, 30);

-- Sample sale
INSERT INTO sales(product_id, quantity, total) VALUES
  (1, 2, 10.00);
